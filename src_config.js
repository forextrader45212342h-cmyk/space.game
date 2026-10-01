import "dotenv/config";

function required(name) {
    const value = process.env[name];

    if (!value) {
        throw new Error(
            `Missing required environment variable: ${name}`
        );
    }

    return value;
}

export const config = {

    port: Number(
        process.env.PORT || 8080
    ),

    databaseUrl:
        required("DATABASE_URL"),

    stripeSecretKey:
        required("STRIPE_SECRET_KEY"),

    stripeWebhookSecret:
        required("STRIPE_WEBHOOK_SECRET"),

    stripeCurrency:
        process.env.STRIPE_CURRENCY || "usd",

    publicBaseUrl:
        required("ORBIT_PUBLIC_BASE_URL"),

    steamAppId:
        process.env.STEAM_APP_ID || "",

    steamPublisherKey:
        process.env.STEAM_PUBLISHER_KEY || "",

    steamPartnerApiBase:
        process.env.STEAM_PARTNER_API_BASE ||
        "https://partner.steam-api.com"
};
