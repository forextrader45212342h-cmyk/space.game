import { config } from "../config.js";


export async function addSteamInventoryItem({

    steamId,
    itemDefId,
    requestId

}) {

    if (
        !config.steamAppId ||
        !config.steamPublisherKey
    ) {

        throw new Error(
            "Steam server credentials are not configured"
        );
    }


    const url =
        `${config.steamPartnerApiBase}` +
        `/IInventoryService/AddItem/v1/`;


    const body =
        new URLSearchParams();


    body.set(
        "key",
        config.steamPublisherKey
    );

    body.set(
        "appid",
        config.steamAppId
    );

    body.set(
        "steamid",
        steamId
    );

    body.set(
        "itemdefid[0]",
        String(itemDefId)
    );


    if (requestId) {

        body.set(
            "requestid",
            String(requestId)
        );
    }


    const response =
        await fetch(
            url,
            {

                method: "POST",

                headers: {
                    "Content-Type":
                        "application/x-www-form-urlencoded"
                },

                body
            }
        );


    if (!response.ok) {

        throw new Error(
            `Steam API HTTP ${response.status}`
        );
    }


    return response.json();
}
