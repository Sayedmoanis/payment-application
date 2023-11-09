#include <stdio.h>
#include "card.h"
#include "terminal.h"
#include "server.h"

int main() {
    // Initialize data structures
    ST_cardData_t cardData;
    ST_terminalData_t terminalData;
    ST_transaction_t transactionData;

    // Get card data
    if (getCardHolderName(&cardData) != CARD_OK ||
        getCardExpiryDate(&cardData) != CARD_OK ||
        getCardPAN(&cardData) != CARD_OK) {
        printf("Error: Invalid card data\n");
        return 1;
    }

    // For testing purposes, set the card expiration date to "22/03"
    strcpy(cardData.cardExpirationDate, "22/03");

    // Get terminal data
    if (getTransactionDate(&terminalData) != TERMINAL_OK ||
        isCardExpired(&cardData, &terminalData) != TERMINAL_OK ||
        getTransactionAmount(&terminalData) != TERMINAL_OK) {
        printf("Error: Invalid terminal data\n");
        return 1;
    }

    // Check if the card PAN is valid
    if (isValidCardPAN(&cardData) != TERMINAL_OK) {
        printf("Error: Invalid card PAN\n");
        return 1;
    }

    // Fill the transaction data
    transactionData.cardHolderData = cardData;
    transactionData.terminalData = terminalData;

    // Process the transaction
    EN_transState_t transactionStatus = recieveTransactionData(&transactionData);

    // Display the transaction result
    switch (transactionStatus) {
        case APPROVED:
            printf("Transaction Approved!\n");
            break;
        case DECLINED_INSUFFICIENT_FUND:
            printf("Transaction Declined: Insufficient Fund\n");
            break;
        case DECLINED_STOLEN_CARD:
            printf("Transaction Declined: Stolen Card\n");
            break;
        case FRAUD_CARD:
            printf("Transaction Declined: Fraud Card\n");
            break;
        case INTERNAL_SERVER_ERROR:
            printf("Transaction Declined: Internal Server Error\n");
            break;
        default:
            printf("Unknown Transaction Status\n");
            break;
    }

    // List saved transactions
    listSavedTransactions();

    return 0;
}
