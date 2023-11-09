// card.c

#include "card.h"
#include <stdio.h>
#include <string.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
    printf("Enter cardholder's name (20-24 characters): ");
    fgets(cardData->cardHolderName, sizeof(cardData->cardHolderName), stdin);
    cardData->cardHolderName[strcspn(cardData->cardHolderName, "\n")] = '\0';

    if (cardData->cardHolderName == NULL || strlen(cardData->cardHolderName) < 20 || strlen(cardData->cardHolderName) > 24) {
        return WRONG_NAME;
    }

    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
    printf("Enter card expiry date (MM/YY): ");
    fgets(cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), stdin);
    cardData->cardExpirationDate[strcspn(cardData->cardExpirationDate, "\n")] = '\0';

    if (cardData->cardExpirationDate == NULL || strlen(cardData->cardExpirationDate) != 5) {
        return WRONG_EXP_DATE;
    }

    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
    printf("Enter Primary Account Number (PAN, 16-19 digits): ");
    fgets(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), stdin);
    cardData->primaryAccountNumber[strcspn(cardData->primaryAccountNumber, "\n")] = '\0';

    if (cardData->primaryAccountNumber == NULL || strlen(cardData->primaryAccountNumber) < 16 || strlen(cardData->primaryAccountNumber) > 19) {
        return WRONG_PAN;
    }

    return CARD_OK;
}
