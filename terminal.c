// terminal.c

#include "terminal.h"
#include <stdio.h>
#include <string.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) {
    printf("Enter transaction date (DD/MM/YYYY): ");
    fgets(termData->transactionDate, sizeof(termData->transactionDate), stdin);
    termData->transactionDate[strcspn(termData->transactionDate, "\n")] = '\0';

    if (termData->transactionDate == NULL || strlen(termData->transactionDate) != 10) {
        return WRONG_DATE;
    }

    return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData) {
    // Assuming the date format is DD/MM/YYYY
    int cardYear = atoi(cardData->cardExpirationDate + 3);
    int termYear = atoi(termData->transactionDate + 6);

    if (cardYear < termYear) {
        return EXPIRED_CARD;
    }

    return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) {
    printf("Enter transaction amount: ");
    scanf("%f", &termData->transAmount);

    if (termData->transAmount <= 0) {
        return INVALID_AMOUNT;
    }

    return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) {
    if (termData->transAmount > termData->maxTransAmount) {
        return EXCEED_MAX_AMOUNT;
    }

    return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount) {
    if (maxAmount <= 0) {
        return INVALID_MAX_AMOUNT;
    }

    termData->maxTransAmount = maxAmount;
    return TERMINAL_OK;
}



EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) {
    // Check if PAN is a Luhn number
    int sum = 0;
    int len = strlen(cardData->primaryAccountNumber);
    int parity = len % 2;

    for (int i = len - 1; i >= 0; i--) {
        int digit = cardData->primaryAccountNumber[i] - '0';

        if (i % 2 == parity) {
            digit *= 2;
            if (digit > 9) {
                digit -= 9;
            }
        }

        sum += digit;
    }

    if (sum % 10 != 0) {
        return INVALID_CARD;
    }

    return TERMINAL_OK;
}
