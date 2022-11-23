#pragma once
#ifndef CARD      // Header File Gaurd
#define CARD

#define MAX_STRING_NAME 24      // setting maximum characters of string name
#define MIN_STRING_NAME 20      // setting minimum characters of string name
#define NUM_STRING_DATE 5       // setting number of characters of string date
#define MAX_PAN 19              // setting maximum number of characters of PAN
#define MIN_PAN 16              // setting minimum number of characters of PAN

typedef unsigned char uint8_t;

typedef enum EN_cardError_t{
	CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;

EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);
#endif // !CARD
