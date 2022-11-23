#include "terminal.h"      // including terminal.h file
#include <stdio.h>         // including stdio.h for input user and printing
#include <string.h>        // including string.h to use strlen() for string length
#include <ctype.h>         // including ctype.h to use isdigit() function
#include <stdlib.h>        // including stdlib.h to use atoi() function
#include <stdint.h>


char year_Transaction[2];
char year_Expiry[2];
char mon_Transaction[2];
char mon_Expiry[2];
int y_T;
int y_E;
int m_T;
int m_E;
int nSum;
int flag;                 // flag to identify second element in Luhn algorithm
int num_m_two;            // temp variable for the multiplied element in Luhn algorithm


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) {
	printf("Please enter the Transaction Date : \n");          // Asking user to enter the transaction date
	gets(termData->transactionDate);                           // scaning the input of the keyboard of the user
	if (termData->transactionDate == NULL) {                   // Checking whether NULL or not
		return WRONG_DATE;
	}
	if (strlen(termData->transactionDate) != STRING_DATE) {    // Checking the length of the String
		return WRONG_DATE;
	}
	for (int i = 0; i < STRING_DATE; i++) {                    // Checking the format
		if (i == 2 || i == 5) {
			if (termData->transactionDate[i] != '/') {
				return WRONG_DATE;
			}
		}
		else{
			if (isdigit(termData->transactionDate[i]) == 0) {
				return WRONG_DATE;
			}
		}
		
	}
	if (termData->transactionDate[0] - '0' > 2 && termData->transactionDate[1]-'0'>0) {
		return WRONG_DATE;
	}
	if (termData->transactionDate[3] - '0' > 0 && termData->transactionDate[4]-'0' > 2) {
		return WRONG_DATE;
	}
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
	year_Transaction[0] = termData.transactionDate[8];
	year_Transaction[1] = termData.transactionDate[9];
	year_Expiry[0] = cardData.cardExpirationDate[3];
	year_Expiry[1] = cardData.cardExpirationDate[4];
	mon_Transaction[0] = termData.transactionDate[3];
	mon_Transaction[1] = termData.transactionDate[4];
	mon_Expiry[0] = cardData.cardExpirationDate[0];
	mon_Expiry[1] = cardData.cardExpirationDate[1];
	y_T = (year_Transaction[0] - '0') * 10 + (year_Transaction[1] - '0');
	y_E = (year_Expiry[0] - '0') * 10 + (year_Expiry[1] - '0');
	m_T = (mon_Transaction[0] - '0') * 10 + (mon_Transaction[1] - '0');
	m_E = (mon_Expiry[0] - '0') * 10 + (mon_Expiry[1] - '0');

	if (y_T > y_E) {
		return EXPIRED_CARD;
	}
	else {
		if (y_T == y_E) {
			if (m_T > m_E) {
				return EXPIRED_CARD;
			}
		}
	}
	return TERMINAL_OK;

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("Please enter the Transaction Amount : \n");          // Asking user to enter the transaction amount
	scanf_s("%f", &termData->transAmount);                       // scaning the input of the keyboard of the user
	
	if (termData->transAmount <= 0) {
		return INVALID_AMOUNT;
	}
	return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount>termData->maxTransAmount) {
		return EXCEED_MAX_AMOUNT;
	}
	return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	printf("Please enter the Maximum Transaction Amount : \n");          // Asking user to enter the maximum transaction amount
	scanf_s("%f", &termData->maxTransAmount);                            // scaning the input of the keyboard of the user

	if (termData->maxTransAmount <= 0) {
		return INVALID_MAX_AMOUNT;
	}
	return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) {
	nSum = 0;
	flag = 0;
	for (int i = strlen(cardData->primaryAccountNumber) - 1; i >= 0; i--) {
		if (flag == 1) {
			num_m_two = 2*(cardData->primaryAccountNumber[i] - '0');
			nSum = nSum + num_m_two / 10;
			nSum = nSum + num_m_two % 10;
			flag = 0;
		}
		else if(flag==0) {
			nSum = nSum + (cardData->primaryAccountNumber[i] - '0');
			flag = 1;
		}
	}
	if (nSum % 10 != 0) {
		return INVALID_CARD;
	}
	return TERMINAL_OK;
}

ST_terminalData_t termDataTest;
ST_cardData_t cardDataTest;
char ExpStr[100];


void getTransactionDateTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: getTransactionDate. \n");
	printf("Test Case  \n");
	EN_terminalError_t enumTest = getTransactionDate(&termDataTest);
	printf("Input Data: %s \n", termDataTest.transactionDate);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == WRONG_DATE) {
		printf("Actual Result: Transaction Date is invalid \n");
	}
	else if (enumTest == TERMINAL_OK) {
		printf("Actual Result: Transaction Date is Valid \n");
	}
	printf("------------------------------------------------------ \n");
}

void isCardExpiredTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: isCardExpired. \n");
	printf("Test Case  \n");
	getTransactionDate(&termDataTest);
	getCardExpiryDate(&cardDataTest);
	EN_terminalError_t enumTest = isCardExpired(cardDataTest,termDataTest);
	printf("Input Data: Transaction Date is %s ", termDataTest.transactionDate);
	printf(" and Expiry Date is %s", cardDataTest.cardExpirationDate);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == EXPIRED_CARD) {
		printf("Actual Result: Card is Expired \n");
	}
	else if (enumTest == TERMINAL_OK) {
		printf("Actual Result: Card is Valid \n");
	}
	printf("------------------------------------------------------ \n");
}

void getTransactionAmountTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: getTransactionAmount. \n");
	printf("Test Case  \n");
	EN_terminalError_t enumTest = getTransactionAmount(&termDataTest);
	printf("Input Data: %f \n", termDataTest.transAmount);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == INVALID_AMOUNT) {
		printf("Actual Result: Transaction Amount is invalid \n");
	}
	else if (enumTest == TERMINAL_OK) {
		printf("Actual Result: Transaction Amount is Valid \n");
	}
	printf("------------------------------------------------------ \n");
}

void isBelowMaxAmountTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: isBelowMaxAmount. \n");
	printf("Test Case  \n");
	getTransactionAmount(&termDataTest);
	setMaxAmount(&termDataTest);
	EN_terminalError_t enumTest = isBelowMaxAmount(&termDataTest);
	printf("Input Data: Transaction Amount is %f ", termDataTest.transAmount);
	printf(" and Maximum Amount is %f", termDataTest.maxTransAmount);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == EXCEED_MAX_AMOUNT) {
		printf("Actual Result: Max Amount Exceeded \n");
	}
	else if (enumTest == TERMINAL_OK) {
		printf("Actual Result: Transaction Amount is allowable \n");
	}
	printf("------------------------------------------------------ \n");
}

void setMaxAmountTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: setMaxAmount. \n");
	printf("Test Case  \n");
	EN_terminalError_t enumTest = setMaxAmount(&termDataTest);
	printf("Input Data: %f \n", termDataTest.maxTransAmount);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == INVALID_MAX_AMOUNT) {
		printf("Actual Result: Maximum Amount is invalid \n");
	}
	else if (enumTest == TERMINAL_OK) {
		printf("Actual Result: Maximum Amount is Valid \n");
	}
	printf("------------------------------------------------------ \n");
}

void isValidCardPANTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: isValidCardPAN. \n");
	printf("Test Case  \n");
	getCardPAN(&cardDataTest);
	EN_terminalError_t enumTest = isValidCardPAN(&cardDataTest);
	printf("Input Data: PAN is %s ", cardDataTest.primaryAccountNumber);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == INVALID_CARD) {
		printf("Actual Result: PAN is not a Luhn number \n");
	}
	else if (enumTest == TERMINAL_OK) {
		printf("Actual Result: PAN is a Luhn number \n");
	}
	printf("------------------------------------------------------ \n");
}