// server.c

#include "server.h"
#include <stdio.h>
#include <string.h>

ST_accountsDB_t validAccounts[] = {
    {1000.0, RUNNING, "12345678901234567890"},
    {500.0, RUNNING, "98765432109876543210"},

    {1500.0, RUNNING, "11112222333344445555"},
    {2000.0, RUNNING, "66667777888899990000"},

};

ST_transaction_t transactions[100] = {0};
uint32_t lastTransactionSequenceNumber = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData) {
    EN_serverError_t accountCheckResult = isValidAccount(&transData->cardHolderData, NULL);
    if (accountCheckResult != SERVER_OK) {
        return FRAUD_CARD;
    }

    EN_serverError_t blockedCheckResult = isBlockedAccount(&transData->cardHolderData);
    if (blockedCheckResult != SERVER_OK) {
        return DECLINED_STOLEN_CARD;
    }

    EN_serverError_t amountCheckResult = isAmountAvailable(&transData->terminalData, NULL);
    if (amountCheckResult != SERVER_OK) {
        return DECLINED_INSUFFICIENT_FUND;
    }

    transData->transState = APPROVED;
    lastTransactionSequenceNumber++;
    transData->transactionSequenceNumber = lastTransactionSequenceNumber;

    EN_serverError_t saveResult = saveTransaction(transData);
    if (saveResult != SERVER_OK) {
        return INTERNAL_SERVER_ERROR;
    }

    return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountReference) {
    for (int i = 0; i < sizeof(validAccounts) / sizeof(validAccounts[0]); i++) {
        if (strcmp(cardData->primaryAccountNumber, validAccounts[i].primaryAccountNumber) == 0) {
            if (accountReference != NULL) {
                *accountReference = validAccounts[i];
            }
            return SERVER_OK;
        }
    }

    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_cardData_t *cardData) {
    for (int i = 0; i < sizeof(validAccounts) / sizeof(validAccounts[0]); i++) {
        if (strcmp(cardData->primaryAccountNumber, validAccounts[i].primaryAccountNumber) == 0) {
            if (validAccounts[i].state == BLOCKED) {
                return BLOCKED_ACCOUNT;
            } else {
                return SERVER_OK;
            }
        }
    }

    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountReference) {
    // Assuming the account is always available for the requested amount
    return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData) {
    for (int i = 0; i < sizeof(transactions) / sizeof(transactions[0]); i++) {
        if (transactions[i].transactionSequenceNumber == 0) {
            transactions[i] = *transData;
            return SERVER_OK;
        }
    }

    return SAVING_FAILED;
}

void listSavedTransactions(void) {
    for (int i = 0; i < sizeof(transactions) / sizeof(transactions[0]); i++) {
        if (transactions[i].transactionSequenceNumber != 0) {
            printf("#########################\n");
            printf("Transaction Sequence Number: %u\n", transactions[i].transactionSequenceNumber);
            printf("Transaction Date: %s\n", transactions[i].terminalData.transactionDate);
            printf("Transaction Amount: %.2f\n", transactions[i].terminalData.transAmount);
            printf("Transaction State: %d\n", transactions[i].transState);
            printf("Terminal Max Amount: %.2f\n", transactions[i].terminalData.maxTransAmount);
            printf("Cardholder Name: %s\n", transactions[i].cardHolderData.cardHolderName);
            printf("PAN: %s\n", transactions[i].cardHolderData.primaryAccountNumber);
            printf("Card Expiration Date: %s\n", transactions[i].cardHolderData.cardExpirationDate);
            printf("#########################\n");
        }
    }
}
