#include "card.h"          // inserting card.h file
#include <stdio.h>         // including stdio.h for input user and printing
#include <string.h>        // including string.h to use strlen() for string length
#include <ctype.h>         // including ctype.h to use isdigit() function


EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {

	printf("Please enter the card holder's name : \n");        // Asking user to enter the card holder name
	gets(cardData->cardHolderName);                            // scaning the input of the keyboard of the user
	if (cardData->cardHolderName == NULL) {                    // checking the number of characters of the string whether
		return WRONG_NAME;                                     // it is within range or not
	}
	if (strlen(cardData->cardHolderName)<MIN_STRING_NAME || strlen(cardData->cardHolderName)>MAX_STRING_NAME){
		return WRONG_NAME;
	}
	for (int i = 0; i < strlen(cardData->cardHolderName);i++) {
		if (isalpha(cardData->cardHolderName[i]) == 0 && isspace(cardData->cardHolderName[i]==0)) {
			return WRONG_NAME;
		}
	}
	return CARD_OK;
}


EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
	
	printf("Please enter the card expiry date : \n");        // Asking user to enter the card expiry data
	gets(cardData->cardExpirationDate);                      // scaning the input of the keyboard of the user

	if (cardData->cardExpirationDate == NULL) {              // checking the constraints of the format of the input
		return WRONG_EXP_DATE;
	}
	if (strlen(cardData->cardExpirationDate) != NUM_STRING_DATE || isdigit(cardData->cardExpirationDate[0])==0 || isdigit(cardData->cardExpirationDate[1]) == 0 || isdigit(cardData->cardExpirationDate[3]) == 0 || isdigit(cardData->cardExpirationDate[4]) == 0 || (cardData->cardExpirationDate[2]) != '/'){
		return WRONG_EXP_DATE;
	}
	if (cardData->cardExpirationDate[0]-'0'>0 && cardData->cardExpirationDate[1]-'0'>2) {
		return WRONG_EXP_DATE;
	}
	return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
	printf("Please enter the primary account number : \n");        // Asking user to enter the primary account number
	gets(cardData->primaryAccountNumber);                          // scaning the input of the keyboard of the user

	if (cardData->primaryAccountNumber == NULL) {                  // checking the constraints of the format of the input
		return WRONG_PAN;
	}

	if ((strlen(cardData->primaryAccountNumber) < 16 || strlen(cardData->primaryAccountNumber) > 19)) {
		return WRONG_PAN;
	}
	for (int i = 0; i < strlen(cardData->primaryAccountNumber); i++) {
		if (isdigit(cardData->primaryAccountNumber[i]) == 0) {
			return WRONG_PAN;
		}
	}
	return CARD_OK;
}

ST_cardData_t cardDataTest;
char ExpStr[100];
void getCardHolderNameTest(void) {
	
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: getCardHolderName. \n");
	printf("Test Case  \n");
	EN_cardError_t enumTest = getCardHolderName(&cardDataTest);
	printf("Input Data: %s \n", cardDataTest.cardHolderName);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == WRONG_NAME) {
		printf("Actual Result: Name is invalid \n");
	}
	else if (enumTest == CARD_OK) {
		printf("Actual Result: Name is Valid \n");
	}
	printf("------------------------------------------------------ \n");
}

void getCardExpiryDateTest(void) {

	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: getCardExpiryDate. \n");
	printf("Test Case  \n");
	EN_cardError_t enumTest = getCardExpiryDate(&cardDataTest);
	printf("Input Data: %s \n", cardDataTest.cardExpirationDate);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == WRONG_EXP_DATE) {
		printf("Actual Result: Expiration Date is invalid \n");
	}
	else if (enumTest == CARD_OK) {
		printf("Actual Result: Expiration Date is Valid \n");
	}
	printf("------------------------------------------------------ \n");
}

void getCardPANTest(void) {

	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: getCardPANTest. \n");
	printf("Test Case  \n");
	EN_cardError_t enumTest = getCardPAN(&cardDataTest);
	printf("Input Data: %s \n", cardDataTest.primaryAccountNumber);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == WRONG_PAN) {
		printf("Actual Result: PAN is invalid \n");
	}
	else if (enumTest == CARD_OK) {
		printf("Actual Result: PAN is Valid \n");
	}
	printf("------------------------------------------------------ \n");
}