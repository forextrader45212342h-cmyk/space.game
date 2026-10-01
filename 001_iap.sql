CREATE TABLE IF NOT EXISTS store_products (
    product_id TEXT PRIMARY KEY,

    display_name TEXT NOT NULL,

    currency CHAR(3) NOT NULL DEFAULT 'usd',

    unit_amount_minor BIGINT NOT NULL
        CHECK (unit_amount_minor >= 0),

    grant_type TEXT NOT NULL
        CHECK (grant_type IN ('coins', 'item')),

    grant_key TEXT NOT NULL,

    grant_quantity BIGINT NOT NULL
        CHECK (grant_quantity > 0),

    active BOOLEAN NOT NULL DEFAULT TRUE,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);


CREATE TABLE IF NOT EXISTS iap_orders (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),

    user_id UUID NOT NULL
        REFERENCES users(id)
        ON DELETE RESTRICT,

    product_id TEXT NOT NULL
        REFERENCES store_products(product_id),

    provider TEXT NOT NULL
        CHECK (provider IN ('stripe', 'steam')),

    provider_reference TEXT NOT NULL,

    amount_minor BIGINT NOT NULL
        CHECK (amount_minor >= 0),

    currency CHAR(3) NOT NULL,

    status TEXT NOT NULL
        CHECK (
            status IN (
                'pending',
                'paid',
                'failed',
                'refunded'
            )
        ),

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    paid_at TIMESTAMPTZ
);


CREATE UNIQUE INDEX IF NOT EXISTS
ux_iap_provider_reference
ON iap_orders(provider, provider_reference);


CREATE TABLE IF NOT EXISTS player_wallets (
    user_id UUID PRIMARY KEY
        REFERENCES users(id)
        ON DELETE CASCADE,

    coins BIGINT NOT NULL DEFAULT 0
        CHECK (coins >= 0),

    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);


CREATE TABLE IF NOT EXISTS player_inventory (
    user_id UUID NOT NULL
        REFERENCES users(id)
        ON DELETE CASCADE,

    item_key TEXT NOT NULL,

    quantity BIGINT NOT NULL DEFAULT 0
        CHECK (quantity >= 0),

    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    PRIMARY KEY (user_id, item_key)
);


CREATE TABLE IF NOT EXISTS iap_ledger (
    id BIGSERIAL PRIMARY KEY,

    order_id UUID NOT NULL
        REFERENCES iap_orders(id),

    user_id UUID NOT NULL
        REFERENCES users(id),

    entry_type TEXT NOT NULL,

    asset_key TEXT NOT NULL,

    quantity BIGINT NOT NULL,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);


CREATE UNIQUE INDEX IF NOT EXISTS
ux_iap_ledger_order_asset
ON iap_ledger(order_id, asset_key);


INSERT INTO store_products
(
    product_id,
    display_name,
    currency,
    unit_amount_minor,
    grant_type,
    grant_key,
    grant_quantity
)
VALUES
(
    'coins_1000',
    '1,000 Orbit Coins',
    'usd',
    999,
    'coins',
    'coins',
    1000
),
(
    'premium_rocket',
    'Premium Rocket',
    'usd',
    4999,
    'item',
    'premium_rocket',
    1
)
ON CONFLICT (product_id)
DO NOTHING;
