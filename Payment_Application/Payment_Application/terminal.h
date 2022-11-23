#pragma once
#ifndef terminal        // Header File Gaurd
#define terminal
#include "../Payment_Application/card.h"
#define STRING_DATE 10



typedef enum EN_terminalError_t {
	TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t;


typedef struct ST_terminalData_t{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData);

#endif // !terminal
