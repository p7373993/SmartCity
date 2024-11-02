#pragma once
#include <iostream>
#include <string.h>
#include <vector>

struct SSelectorType
{
    int Type = 0;
    int MaxElIndex = 0;
    float Elemental[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    int error = 0;
};

struct APData
{
    char BuildingName[200];
    float latitude;
    float longitude;
    int floorInfo;
    int ApartIndex;
};

struct SaleData
{
    long long articleNo;
    int dealOrWarrantPrc;
    char tradeTypeName[200];
    char realtorName[200];
    int floorInfo;
    int ApartIndex;
};

struct Complex
{
    int ComplexIndex;
    char RoadNameAddress[200];
    char NormalAddress[200];
    float latitude;
    float longitude;
};
struct DisAndPRE
{
    float Distance;
    float PRE;
};
struct SearchStruct
{
    FString BuildingName = "";
    FString tag = "";
    int index = 0;
};
struct TextStruct
{
    FString BuildingName = "";
    FString BuildingAddress = "";
};