#include "app.h"
#include <stdio.h>         // including stdio.h for input user and printing
#include <string.h>        // including string.h to use strlen() for string length
#include <ctype.h>         // including ctype.h to use isdigit() function
void appStart(void) {


	ST_transaction_t trans_Data;
	ST_cardData_t *card_Data = &trans_Data.cardHolderData;
	ST_terminalData_t *term_Data = &trans_Data.terminalData;

	while (!(getCardHolderName(card_Data)==CARD_OK));               // wait until user gives the holder name
	while (!(getCardExpiryDate(card_Data) == CARD_OK));             // wait until user gives the expiry data
	while (!(getCardPAN(card_Data)==CARD_OK));                      // wait until user gives the PAN

	if (isValidCardPAN(card_Data) == INVALID_CARD) {                // check if the given PAN is valid or not
		printf("This given PAN is wrong!");
		return;
	}
	while (getTransactionDate(term_Data));                          // wait until user gives Transaction Date
	if (isCardExpired(*card_Data, *term_Data)==EXPIRED_CARD) {      // Check if it is expired or not
		printf("Unfortunately, This card is expired, Please re-activate the card and try again later!");
		return;
	}
	while (setMaxAmount(term_Data));                                // wait until setting maximum amount
	while (getTransactionAmount(term_Data));                        // wait until getting Transaction amount
	if (isBelowMaxAmount(term_Data)==EXCEED_MAX_AMOUNT) {           // Check if it exceeds maximum amount or not
		printf("Unfortunately, The amount required is more than the Maximum amount allowable!");
		return;
	}
	switch (recieveTransactionData(&trans_Data))                    // Recieve your transaction data and check which case of the following
	{
	case FRAUD_CARD:
		printf("Unfortunately, This card does not exist!");
		break;
	case DECLINED_STOLEN_CARD:
		printf("Unfortunately, This card is blocked!");
		break;
	case DECLINED_INSUFFECIENT_FUND:
		printf("Unfortunately, You have insuffecient balance!");
		break;
	case INTERNAL_SERVER_ERROR:
		printf("Unfortunately, Process failed due to an internal error!");
		break;
	default:
		printf("Process Succeeded!");
		break;
	}
	return;
}