import { withTransaction } from "./db.js";
import { getProduct } from "./catalog.js";


export async function createPendingOrder({

    userId,
    productId,
    provider,
    providerReference,
    amountMinor,
    currency

}) {

    const product =
        await getProduct(productId);

    if (!product) {
        throw new Error(
            "Unknown or inactive product"
        );
    }


    /*
     * Client/provider se aane wali amount ko
     * server catalog ke against verify karte hain.
     */

    if (
        product.unit_amount_minor !==
            Number(amountMinor)
        ||

        product.currency.toLowerCase() !==
            String(currency).toLowerCase()
    ) {

        throw new Error(
            "Provider amount/currency does not match catalog"
        );
    }


    return withTransaction(
        async (client) => {

            const result =
                await client.query(
                    `
                    INSERT INTO iap_orders
                    (
                        user_id,
                        product_id,
                        provider,
                        provider_reference,
                        amount_minor,
                        currency,
                        status
                    )

                    VALUES
                    (
                        $1,
                        $2,
                        $3,
                        $4,
                        $5,
                        $6,
                        'pending'
                    )

                    ON CONFLICT
                    (
                        provider,
                        provider_reference
                    )

                    DO UPDATE SET
                        provider_reference =
                            EXCLUDED.provider_reference

                    RETURNING *
                    `,
                    [
                        userId,
                        productId,
                        provider,
                        providerReference,
                        amountMinor,
                        currency.toLowerCase()
                    ]
                );

            return result.rows[0];
        }
    );
}


/*
 * Payment verified hone ke baad
 * EXACTLY ONCE item/coins grant karta hai.
 */

export async function fulfillPaidOrder(
    provider,
    providerReference
) {

    return withTransaction(
        async (client) => {

            const orderResult =
                await client.query(
                    `
                    SELECT
                        o.*,
                        p.grant_type,
                        p.grant_key,
                        p.grant_quantity

                    FROM iap_orders o

                    JOIN store_products p
                      ON p.product_id =
                         o.product_id

                    WHERE
                        o.provider = $1
                    AND
                        o.provider_reference = $2

                    FOR UPDATE
                    `,
                    [
                        provider,
                        providerReference
                    ]
                );


            const order =
                orderResult.rows[0];


            if (!order) {

                throw new Error(
                    "Order not found"
                );
            }


            /*
             * Idempotency protection.
             *
             * Stripe same webhook dobara bheje
             * to coins/item dobara nahi milenge.
             */

            if (order.status === "paid") {

                return {
                    alreadyFulfilled: true,
                    order
                };
            }


            await client.query(
                `
                UPDATE iap_orders

                SET
                    status = 'paid',
                    paid_at = NOW()

                WHERE id = $1
                `,
                [order.id]
            );


            /*
             * COINS
             */

            if (
                order.grant_type ===
                "coins"
            ) {

                await client.query(
                    `
                    INSERT INTO player_wallets
                    (
                        user_id,
                        coins
                    )

                    VALUES
                    (
                        $1,
                        $2
                    )

                    ON CONFLICT (user_id)

                    DO UPDATE SET

                        coins =
                            player_wallets.coins
                            + EXCLUDED.coins,

                        updated_at = NOW()
                    `,
                    [
                        order.user_id,
                        order.grant_quantity
                    ]
                );
            }


            /*
             * ITEM
             */

            else if (
                order.grant_type ===
                "item"
            ) {

                await client.query(
                    `
                    INSERT INTO player_inventory
                    (
                        user_id,
                        item_key,
                        quantity
                    )

                    VALUES
                    (
                        $1,
                        $2,
                        $3
                    )

                    ON CONFLICT
                    (
                        user_id,
                        item_key
                    )

                    DO UPDATE SET

                        quantity =
                            player_inventory.quantity
                            + EXCLUDED.quantity,

                        updated_at = NOW()
                    `,
                    [
                        order.user_id,
                        order.grant_key,
                        order.grant_quantity
                    ]
                );
            }


            /*
             * Permanent audit record.
             */

            await client.query(
                `
                INSERT INTO iap_ledger
                (
                    order_id,
                    user_id,
                    entry_type,
                    asset_key,
                    quantity
                )

                VALUES
                (
                    $1,
                    $2,
                    $3,
                    $4,
                    $5
                )

                ON CONFLICT
                (
                    order_id,
                    asset_key
                )

                DO NOTHING
                `,
                [
                    order.id,
                    order.user_id,
                    "purchase_grant",
                    order.grant_key,
                    order.grant_quantity
                ]
            );


            return {
                alreadyFulfilled: false,
                order
            };
        }
    );
}
