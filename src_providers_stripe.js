import Stripe from "stripe";

import { config } from "../config.js";

import {
    getProduct
} from "../catalog.js";

import {
    createPendingOrder,
    fulfillPaidOrder
} from "../iapService.js";


export const stripe =
    new Stripe(
        config.stripeSecretKey
    );


export async function createCheckoutSession({

    userId,
    productId

}) {

    /*
     * Product price SERVER se read hoga.
     * Client apni price nahi bhej sakta.
     */

    const product =
        await getProduct(productId);


    if (!product) {

        throw new Error(
            "Product not found"
        );
    }


    const session =
        await stripe.checkout.sessions.create({

            mode: "payment",

            line_items: [
                {
                    price_data: {

                        currency:
                            product.currency
                            .toLowerCase(),

                        product_data: {

                            name:
                                product.display_name
                        },

                        unit_amount:
                            product.unit_amount_minor
                    },

                    quantity: 1
                }
            ],

            metadata: {

                user_id: userId,

                product_id:
                    product.product_id
            },

            success_url:
                `${config.publicBaseUrl}` +
                `/store/success` +
                `?session_id=` +
                `{CHECKOUT_SESSION_ID}`,

            cancel_url:
                `${config.publicBaseUrl}` +
                `/store/cancel`
        });


    await createPendingOrder({

        userId,

        productId,

        provider:
            "stripe",

        providerReference:
            session.id,

        amountMinor:
            product.unit_amount_minor,

        currency:
            product.currency
    });


    return session;
}


/*
 * Stripe signed webhook processor.
 */

export async function handleStripeWebhook(
    rawBody,
    signature
) {

    /*
     * constructEvent verifies the Stripe signature.
     */

    const event =
        stripe.webhooks.constructEvent(
            rawBody,
            signature,
            config.stripeWebhookSecret
        );


    switch (event.type) {

        case "checkout.session.completed":

        case "checkout.session.async_payment_succeeded":
        {

            const session =
                event.data.object;


            /*
             * Do NOT grant the item merely because
             * checkout completed.
             */

            if (
                session.payment_status !==
                "paid"
            ) {

                return {
                    received: true,
                    fulfilled: false
                };
            }


            const result =
                await fulfillPaidOrder(
                    "stripe",
                    session.id
                );


            return {

                received: true,

                fulfilled:
                    !result.alreadyFulfilled
            };
        }


        default:

            return {
                received: true,
                ignored: true
            };
    }
}
