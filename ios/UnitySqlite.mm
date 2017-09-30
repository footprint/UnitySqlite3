//
//  UnitySqlite.m
//  Unity-iPhone
//
//  Created by footprint on 7/4/17.
//
//

#import "UnitySqlite.h"
#include "SqliteHelper.hpp"

@implementation UnitySqlite

@end

//helper methods
extern char* MakeStringCopy (const char* string);
extern NSString* CreateNSString (const char* string);

extern "C" {
    bool openDB(const char* path) {
        return SqliteHelper::getInstance()->openDB(path);
    }
    
    void closeDB() {
        SqliteHelper::getInstance()->closeDB();
    }
    
    bool executeSql(const char* sql) {
        return SqliteHelper::getInstance()->executeSql(sql);
    }
    
    bool prepareStatement(const char* key, const char* prepare) {
        return SqliteHelper::getInstance()->prepareStatement(key, prepare);
    }
    
    bool bindNull(const char* key, int index) {
        return SqliteHelper::getInstance()->bindNull(key, index);
    }
    
    bool bindInt(const char* key, int index, int values) {
        return SqliteHelper::getInstance()->bindInt(key, index, values);
    }
    
    bool bindDouble(const char* key, int index, double values) {
        return SqliteHelper::getInstance()->bindDouble(key, index, values);
    }
    
    bool bindText(const char* key, int index, const char* text) {
        return SqliteHelper::getInstance()->bindText(key, index, text);
    }
    
    bool resetPrepare(const char* key) {
        return SqliteHelper::getInstance()->resetPrepare(key);
    }
    
    bool exePrepareOnce(const char* key) {
        return SqliteHelper::getInstance()->exePrepareOnce(key);
    }
    
    //@result:json string
    const char* exePrepareQuery(const char* key) {
        return MakeStringCopy(SqliteHelper::getInstance()->exePrepareQuery(key).c_str());
    }
    
}
