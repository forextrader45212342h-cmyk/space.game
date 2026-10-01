import { pool } from "./db.js";


export async function getProduct(
    productId
) {

    const result =
        await pool.query(
            `
            SELECT
                product_id,
                display_name,
                currency,
                unit_amount_minor,
                grant_type,
                grant_key,
                grant_quantity

            FROM store_products

            WHERE product_id = $1
              AND active = TRUE
            `,
            [productId]
        );

    return result.rows[0] || null;
}
