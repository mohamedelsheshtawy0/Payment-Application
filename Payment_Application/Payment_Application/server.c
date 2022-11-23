#include "server.h"
#include <stdio.h>         // including stdio.h for input user and printing
#include <string.h>        // including string.h to use strlen() for string length
#include <ctype.h>         // including ctype.h to use isdigit() function
#define Numelement_Transaction_Data 255
#define Numelement_Account_Data 255


extern ST_accountsDB_t accountsDB[Numelement_Account_Data] = {
	{5000.0,RUNNING, "607833656635906773"},
	{4000.5,BLOCKED, "005768153188661175"},
	{3000.2,RUNNING, "075982963293069095"},
	{2000.12,RUNNING, "611153338528212031"},
	{1000.334,BLOCKED, "910398034808780349"},
	{1002.4,RUNNING, "053325461606192194"},
	{999.2,RUNNING, "742772749714646812"},
	{324.2,BLOCKED, "209325396074423440"},
	{1231.4,RUNNING, "851079961065873585"},
	{1253.4,RUNNING, "442875351414250640"},
	{5123.5,RUNNING, "615903891876676888"},
	{6345.1,BLOCKED, "59022961351481653"},
	{6345.6,RUNNING, "91119474574056192"},
	{1241.7,RUNNING, "89928622813608190"},
	{4761.3,BLOCKED, "78687103102360615"},
	{125.7,RUNNING, "8316351323704972"},
	{214.6,BLOCKED, "5006447651585560"},
	{1251.3,BLOCKED, "3870771107430131"},
	{151.6,RUNNING, "0354025546355735"}
};

extern ST_transaction_t transactions_Data_base[Numelement_Transaction_Data] = { 0 };

ST_accountsDB_t ReferenceAccount;

int index = -1;
int seq_Num = 0;



EN_transState_t recieveTransactionData(ST_transaction_t *transData) {
	if (isValidAccount(&(transData->cardHolderData),&ReferenceAccount)==ACCOUNT_NOT_FOUND) {       // check validation of Luhn number
		return FRAUD_CARD;
	}
	if (isBlockedAccount(&ReferenceAccount)==BLOCKED_ACCOUNT) {                                    // check if it is blocked or not
		transactions_Data_base[seq_Num].transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}
	if (isAmountAvailable(&(transData->terminalData),&ReferenceAccount)==LOW_BALANCE) {            // check the availability of the amount
		transactions_Data_base[seq_Num].transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	if (saveTransaction(transData)==SERVER_OK) {                                                   // save the transaction whether it is
		accountsDB[seq_Num].balance = accountsDB[seq_Num].balance - transData->terminalData.transAmount;   // approved or there is 
		return APPROVED;                                                                                   // an internal error
	}
	transactions_Data_base[seq_Num].transState = INTERNAL_SERVER_ERROR;
	return INTERNAL_SERVER_ERROR;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountReference) {
	for (int i = 0; i < Numelement_Account_Data; i++) {                                           // Loop over the accounts data base 
		if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0) {    // and compare between their PANs
			index = i;
			break;
		}
	}
	if (index == -1) {
		accountReference = NULL;
		return ACCOUNT_NOT_FOUND;
	}
	*accountReference = accountsDB[index];
	index = -1;
	return SERVER_OK;

}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountReference) {             // check state whether blocked or not
	if (accountReference->state == BLOCKED) {
		return BLOCKED_ACCOUNT;
	}
	return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountReference) {

	if (termData->transAmount > accountReference->balance) {
		return LOW_BALANCE;
	}
	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData) {
	seq_Num++;
	if (seq_Num < 255) {
		transactions_Data_base[seq_Num-1] = *transData;
		transactions_Data_base[seq_Num-1].transactionSequenceNumber = seq_Num;
		transactions_Data_base[seq_Num-1].transState = transData->transState;
		listSavedTransactions();
		return SERVER_OK;
	}
	return SAVING_FAILED;
}


void listSavedTransactions(void) {
	if (seq_Num == 0) {
		printf("No transactions Found!");
	}
	else {
		for (int i = 0; i < seq_Num; i++) {
			printf("#########################################################\n");
			printf("Transaction Sequence Number: %i \n",transactions_Data_base[i].transactionSequenceNumber);
			printf("Transaction Date: %s \n",transactions_Data_base[i].terminalData.transactionDate);
			printf("Transaction Amount: %f \n", transactions_Data_base[i].terminalData.transAmount);
			switch (transactions_Data_base[i].transState) {
			case DECLINED_INSUFFECIENT_FUND:
				printf("Transaction State: DECLINED_INSUFFECIENT_FUNG \n");
				break;
			case DECLINED_STOLEN_CARD:
				printf("Transaction State: DECLINED_STOLEN_CARD \n");
				break;
			case FRAUD_CARD:
				printf("Transaction State: FRAUD CARD \n");
				break;
			case INTERNAL_SERVER_ERROR:
				printf("Transaction State: INTERNAL_SERVER_ERROR \n");
				break;
			default:
				printf("Transaction State: APPROVED \n");
			}
			printf("Transaction Max Amount: %f \n", transactions_Data_base[i].terminalData.maxTransAmount);
			printf("Card Holder Name: %s \n", transactions_Data_base[i].cardHolderData.cardHolderName);
			printf("PAN: %s \n", transactions_Data_base[i].cardHolderData.primaryAccountNumber);
			printf("Card Expiration Date: %s \n", transactions_Data_base[i].cardHolderData.cardExpirationDate);
			printf("#########################################################\n");
		}
	}
	return;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
	for (int i = 0; i < seq_Num; i++) {
		if (seq_Num == transactions_Data_base[i].transactionSequenceNumber) {
			*transData = transactions_Data_base[i];
			return SERVER_OK;
		}
	}
	return TRANSACTION_NOT_FOUND;
}

ST_terminalData_t termDataTest;
ST_cardData_t cardDataTest;
ST_transaction_t transDataTest;
char ExpStr[100];
ST_accountsDB_t referenceAcccountTest;

void recieveTransactionDataTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: recieveTransaction. \n");
	printf("Test Case  \n");
	getCardHolderName(&cardDataTest);
	getCardExpiryDate(&cardDataTest);
	getCardPAN(&cardDataTest);
	getTransactionAmount(&termDataTest);
	getTransactionDate(&termDataTest);
	transDataTest.cardHolderData = cardDataTest;
	transDataTest.terminalData = termDataTest;
	switch (recieveTransactionData(&(transDataTest)))
	{
	case FRAUD_CARD:
		printf("Unfortunately, This card does not exist! \n");
		break;
	case DECLINED_STOLEN_CARD:
		printf("Unfortunately, This card is blocked! \n");
		break;
	case DECLINED_INSUFFECIENT_FUND:
		printf("Unfortunately, You have insuffecient balance! \n");
		break;
	case INTERNAL_SERVER_ERROR:
		printf("Unfortunately, Process failed due to an internal error! \n");
		break;
	default:
		printf("Process Succeeded! \n");
		break;
	}
	printf("------------------------------------------------------ \n");
	return;
}

void isValidAccountTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: isValidAccount. \n");
	printf("Test Case  \n");
	getCardHolderName(&cardDataTest);
	getCardExpiryDate(&cardDataTest);
	getCardPAN(&cardDataTest);
	EN_serverError_t enumTest = isValidAccount(&cardDataTest,&referenceAcccountTest);
	printf("Input Data:\n Name is %s \n", cardDataTest.cardHolderName);
	printf("Expiry Date is %s \n", cardDataTest.cardExpirationDate);
	printf("PAN is %s \n", cardDataTest.primaryAccountNumber);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == ACCOUNT_NOT_FOUND) {
		printf("Actual Result: Account is not found \n");
	}
	else if (enumTest == SERVER_OK) {
		printf("Actual Result: Account exists with the following data \n");
		printf("Balance is %f \n",referenceAcccountTest.balance);
		if (referenceAcccountTest.state == BLOCKED) {
			printf("State is Blocked \n");
		}
		else {
			printf("State is Running \n");
		}
		printf("PAN is %s \n", referenceAcccountTest.primaryAccountNumber);
	}
	printf("------------------------------------------------------ \n");
}

void isBlockedAccountTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: isBlockedAccount. \n");
	printf("Test Case  \n");
	getCardHolderName(&cardDataTest);
	getCardExpiryDate(&cardDataTest);
	getCardPAN(&cardDataTest);
	EN_serverError_t enumTest = isValidAccount(&cardDataTest, &referenceAcccountTest);
	printf("Input Data:\n Name is %s \n", cardDataTest.cardHolderName);
	printf("Expiry Date is %s \n", cardDataTest.cardExpirationDate);
	printf("PAN is %s \n", cardDataTest.primaryAccountNumber);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == ACCOUNT_NOT_FOUND) {
		printf("Actual Result: Account is not found \n");
	}
	else if (enumTest == SERVER_OK) {
		if (isBlockedAccount(&referenceAcccountTest) == BLOCKED_ACCOUNT) {
			printf("Actual Result : Account is Blocked \n");
		}
		else if (isBlockedAccount(&referenceAcccountTest) == SERVER_OK) {
			printf("Actual Result : Account is Running \n");
		}
	}
	printf("------------------------------------------------------ \n");
}

void isAmountAvailableTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: isAmountAvailable. \n");
	printf("Test Case  \n");
	getCardHolderName(&cardDataTest);
	getCardExpiryDate(&cardDataTest);
	getCardPAN(&cardDataTest);
	EN_serverError_t enumTest = isValidAccount(&cardDataTest, &referenceAcccountTest);
	printf("Input Data:\n Name is %s \n", cardDataTest.cardHolderName);
	printf("Expiry Date is %s \n", cardDataTest.cardExpirationDate);
	printf("PAN is %s \n", cardDataTest.primaryAccountNumber);
	gets(ExpStr);
	printf("Expected Result: %s \n", ExpStr);
	if (enumTest == ACCOUNT_NOT_FOUND) {
		printf("Actual Result: Account is not found \n");
	}
	else if (enumTest == SERVER_OK) {
		getTransactionAmount(&termDataTest);
		getTransactionDate(&termDataTest);
		setMaxAmount(&termDataTest);
		if (isAmountAvailable(&termDataTest,&referenceAcccountTest) == LOW_BALANCE) {
			printf("Actual Result : Balance is insuffecient \n");
		}
		else if (isAmountAvailable(&termDataTest,&referenceAcccountTest) == SERVER_OK) {
			printf("Actual Result : Required amount is available \n");
		}
	}
	printf("------------------------------------------------------ \n");
}

void saveTransactionTest(void) {
	printf("Tester Name: Mohamed Amr Elsheshtawy. \n");
	printf("Function Name: saveTransaction. \n");
	printf("Test Case  \n");
	getCardHolderName(&cardDataTest);
	getCardExpiryDate(&cardDataTest);
	getCardPAN(&cardDataTest);
	getTransactionAmount(&termDataTest);
	getTransactionDate(&termDataTest);
	setMaxAmount(&termDataTest);
	transDataTest.cardHolderData = cardDataTest;
	transDataTest.terminalData = termDataTest;
	EN_serverError_t enumTest = saveTransaction(&transDataTest);
	printf("------------------------------------------------------ \n");
	return;
}