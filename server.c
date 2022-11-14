#include "std_types.h"
#include "stdlib.h"
#include "stdio.h"
#include "server.h"

//typedef enum EN_transState_t
//{
//	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
//}EN_transState_t;

#define database 255
uint16_t DBIndex;


ST_accountsDB_t accountsDB[database] = {
	{20000, BLOCKED ,"123456789123456789"},
	{50000, RUNNING ,"987654321987654321"},
	{30000, BLOCKED ,"135791357913579135"},
	{60000, RUNNING ,"124680246802468024"} };


ST_transaction_t transactionsDB[255] = { {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t accountRefrence;
	
	if (isValidAccount(&transData->cardHolderData,&accountRefrence) == ACCOUNT_NOT_FOUND)
		return FRAUD_CARD;
	else
	{

		if (isBlockedAccount(&accountRefrence) == BLOCKED_ACCOUNT)
			return DECLINED_STOLEN_CARD;
		else
		{
			if (isAmountAvailable(&transData->terminalData,&accountRefrence) == LOW_BALANCE)
				return DECLINED_INSUFFECIENT_FUND;
			else
			{
				if ((SAVING_FAILED == saveTransaction(transData)))
				{
					return INTERNAL_SERVER_ERROR;
				}
				else
				{
					accountsDB[DBIndex].balance -= transData->terminalData.transAmount;
					printf("NEW BALANCE = %f\n", accountsDB[DBIndex].balance);
					return APPROVED;
				}
			}
		}
	}
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
	for (int i = 0; i < database; i++) {
		if (!strcmp(cardData->primaryAccountNumber,accountsDB[i].primaryAccountNumber)) {
			*accountRefrence = accountsDB[i];
			DBIndex = i;
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
	}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
 {
		/*
			This function will take a reference to an existing account in the database.
			It checks if the account is blocked or not.
			If the account is blocked, will return BLOCKED_ACCOUNT, else will return SERVER_OK.
		*/

	if (accountRefrence->state == BLOCKED)
		return BLOCKED_ACCOUNT;
	else
		return SERVER_OK;

}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
	if (termData->transAmount < accountRefrence->balance) {
		return SERVER_OK;
	}
	else return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	EN_serverError_t saveTransactions_State = APPROVED;

	static uint16_t TransactionsSequenceNumber = 0;

	if ((TransactionsSequenceNumber > database))
	{
		saveTransactions_State = SAVING_FAILED;
	}
	else
	{
		transactionsDB[TransactionsSequenceNumber].cardHolderData = transData->cardHolderData;
		transactionsDB[TransactionsSequenceNumber].terminalData = transData->terminalData;
		transactionsDB[TransactionsSequenceNumber].transState = transData->transState;
		transactionsDB[TransactionsSequenceNumber].transState = TransactionsSequenceNumber;
		transData->transactionSequenceNumber = TransactionsSequenceNumber;

		++TransactionsSequenceNumber;
	}

	return saveTransactions_State;
}




