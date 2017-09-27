//
//  SqliteHelper.hpp
//  Unity-iPhone
//
//  Created by footprint on 7/4/17.
//
//

#ifndef SqliteHelper_hpp
#define SqliteHelper_hpp

#include <string>
#include <sqlite3.h>
#include <map>

class SqliteHelper {
    sqlite3* pdb;
    std::string db_path;
    std::map<std::string, sqlite3_stmt*> maps;
public:
    static SqliteHelper* getInstance();
    
    SqliteHelper();
    ~SqliteHelper();
    
    void closeDB();
    
    bool openDB(const std::string path);
    
    bool executeSql(const std::string sql);
    
    bool prepareStatement(const std::string key, const std::string prepare);
    
    bool bindNull(const std::string key, int index);
    bool bindInt(const std::string key, int index, int values);
    bool bindDouble(const std::string key, int index, double values);
    bool bindText(const std::string key, int index, const std::string text);
    
    bool resetPrepare(const std::string key);
    bool exePrepareOnce(const std::string key);
   
    //@result:json string
    std::string exePrepareQuery(const std::string key);
};

#endif /* SqliteHelper_hpp */
