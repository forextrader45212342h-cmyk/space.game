import express from "express";
import { pool } from "./db.js";
import { getProduct } from "./catalog.js";

import {
    createCheckoutSession,
    handleStripeWebhook
} from "./providers/stripe.js";

import { config } from "./config.js";


const app =
    express();


/*
 * IMPORTANT:
 *
 * Stripe webhook ko RAW body chahiye.
 *
 * Is route se pehle express.json()
 * nahi lagana.
 */

app.post(
    "/webhooks/stripe",

    express.raw({
        type: "application/json"
    }),

    async (req, res) => {

        try {

            const signature =
                req.headers[
                    "stripe-signature"
                ];


            if (!signature) {

                return res.status(400)
                    .json({
                        error:
                            "Missing Stripe signature"
                    });
            }


            await handleStripeWebhook(

                req.body,

                signature
            );


            return res.json({
                received: true
            });

        } catch (error) {

            console.error(
                "Stripe webhook error:",
                error.message
            );


            return res.status(400)
                .json({
                    error:
                        "Webhook verification/processing failed"
                });
        }
    }
);


/*
 * Normal API JSON parser.
 */

app.use(
    express.json({
        limit: "32kb"
    })
);


/*
 * Health check.
 */

app.get(
    "/health",

    async (_req, res) => {

        try {

            await pool.query(
                "SELECT 1"
            );

            res.json({
                ok: true
            });

        } catch {

            res.status(503)
                .json({
                    ok: false
                });
        }
    }
);


/*
 * CREATE CHECKOUT
 *
 * Production mein userId directly client se
 * accept mat karna.
 *
 * Existing login/JWT middleware:
 *
 * req.user.id
 *
 * se user ID lena.
 */

app.post(
    "/iap/checkout",

    async (req, res) => {

        try {

            const {
                userId,
                productId
            } = req.body;


            if (
                !isUuid(userId) ||
                typeof productId !==
                    "string"
            ) {

                return res.status(400)
                    .json({
                        error:
                            "Invalid userId/productId"
                    });
            }


            const session =
                await createCheckoutSession({

                    userId,

                    productId
                });


            return res.json({

                checkoutSessionId:
                    session.id,

                checkoutUrl:
                    session.url
            });

        } catch (error) {

            console.error(
                "Checkout error:",
                error.message
            );


            return res.status(400)
                .json({
                    error:
                        error.message
                });
        }
    }
);


/*
 * STORE PRODUCT INFO
 */

app.get(
    "/store/product/:productId",

    async (req, res) => {

        const product =
            await getProduct(
                req.params.productId
            );


        if (!product) {

            return res.status(404)
                .json({
                    error:
                        "Product not found"
                });
        }


        res.json(product);
    }
);


function isUuid(value) {

    return (
        typeof value === "string" &&

        /^[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$/i
            .test(value)
    );
}


app.listen(
    config.port,

    () => {

        console.log(
            `Project Orbit IAP server listening on :${config.port}`
        );
    }
);
