#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define ATTEMPTS_AMOUNT 3
#define SQL_SIZE 600
#define OWNER_RIGHTS 1
#define CLIENT_RIGHTS 0

extern const char* ownerLogin;
extern const char* ownerPassword;
extern int accessRights;


void Authorization(void);
int DBrequest(sqlite3* db, char* sql);
void AllCompactTradeInfo(sqlite3* db);
void GetCostAndAmountonPeriod(sqlite3* db, int id, char* date1, char* date2);
void GetMostSoldCompactInfo(sqlite3* db);
void GetMostPopularPerformerTradeInfo(sqlite3* db);
void AllAuthorsInfo(sqlite3* db);
void GetPeriodTrade(sqlite3* db, char* date1, char* date2);
void GetCompactSoldInfo(sqlite3* db, int id);
void Requests(sqlite3* db, int accessRights);
int checkDate(char date[]);
int Insert(sqlite3 *db, char *zErrMsg, int ret);
int Delete(sqlite3 *db, char *zErrMsg, int ret);
int Update(sqlite3* db, char *zErrMsg, int ret);
char* GetLastDate(sqlite3* db);
int HaveDisks(char *date, sqlite3* db);
