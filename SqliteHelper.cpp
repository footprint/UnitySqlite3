//
//  SqliteHelper.cpp
//  Unity-iPhone
//
//  Created by footprint on 7/4/17.
//
//

#include "SqliteHelper.hpp"
#include "rapidjson.h"
#include "document.h"
#include "pointer.h"
#include "stringbuffer.h"
#include "writer.h"

using namespace FGJSON_NAMESPACE;

static SqliteHelper* instance = nullptr;

SqliteHelper* SqliteHelper::getInstance() {
    if (nullptr == instance) {
        instance = new SqliteHelper();
    }
    return instance;
}


SqliteHelper::SqliteHelper() :
pdb(nullptr) {
    
}

SqliteHelper::~SqliteHelper() {
    closeDB();
}

void SqliteHelper::closeDB() {
    //statements
    for (std::map<std::string,sqlite3_stmt*>::iterator it = maps.begin(); it != maps.end(); ++it) {
        sqlite3_finalize(it->second);
    }
    maps.clear();
    
    //db
    if (nullptr != pdb) {
        sqlite3_close(pdb);
        pdb = nullptr;
    }
}

bool SqliteHelper::openDB(const std::string path) {
    if (db_path != path) {
        closeDB();
    }
    if (nullptr == pdb) {
        if (SQLITE_OK == sqlite3_open(path.c_str(), &pdb)) {
            db_path = path;
        }else {
            return false;
        }
    }
    return true;
}

bool SqliteHelper::executeSql(const std::string sql) {
    if (nullptr == pdb) return false;
    
    return SQLITE_OK == sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
}

bool SqliteHelper::prepareStatement(const std::string key, const std::string prepare) {
    if (nullptr != maps[key]) {
        return true;
    }
    
    sqlite3_stmt* pstmt = nullptr;
    if (SQLITE_OK == sqlite3_prepare_v2(pdb, prepare.c_str(), -1, &pstmt, nullptr)) {
        maps[key] = pstmt; //work!
//        maps.insert(std::make_pair(key, pstmt)); //not work!
        return true;
    }
    return false;
}

bool SqliteHelper::bindNull(const std::string key, int index) {
    sqlite3_stmt* pstmt = maps[key];
    if (nullptr != pstmt) {
        return SQLITE_OK == sqlite3_bind_null(pstmt, index);
    }
    return  false;
}

bool SqliteHelper::bindInt(const std::string key, int index, int values) {
    sqlite3_stmt* pstmt = maps[key];
    if (nullptr != pstmt) {
        return SQLITE_OK == sqlite3_bind_int(pstmt, index, values);
    }
    return  false;
}

bool SqliteHelper::bindDouble(const std::string key, int index, double values) {
    sqlite3_stmt* pstmt = maps[key];
    if (nullptr != pstmt) {
        return SQLITE_OK == sqlite3_bind_double(pstmt, index, values);
    }
    return  false;
}

bool SqliteHelper::bindText(const std::string key, int index, const std::string text) {
    sqlite3_stmt* pstmt = maps[key];
    if (nullptr != pstmt) {
        return SQLITE_OK == sqlite3_bind_text(pstmt, index, text.c_str(), -1, SQLITE_TRANSIENT);
    }
    return  false;
}

bool SqliteHelper::resetPrepare(const std::string key) {
    sqlite3_stmt* pstmt = maps[key];
    if (nullptr != pstmt) {
        return SQLITE_OK == sqlite3_reset(pstmt);
    }
    return false;
}

bool SqliteHelper::exePrepareOnce(const std::string key) {
    sqlite3_stmt* pstmt = maps[key];
    if (nullptr != pstmt) {
        if (SQLITE_DONE == sqlite3_step(pstmt)) {
            return SQLITE_OK == sqlite3_reset(pstmt);
        }
    }
    return false;
}


std::string SqliteHelper::exePrepareQuery(const std::string key) {
    sqlite3_stmt* pstmt = maps[key];
    
    if (nullptr != pstmt) {
        Document doc;
        doc.Parse("[]"); //init as an array
      
        while (SQLITE_ROW == sqlite3_step(pstmt)) {
            Value rowValue(kObjectType);
            
            int count = sqlite3_column_count(pstmt);
            for (int i = 0; i < count; ++i) {
                int type = sqlite3_column_type(pstmt, i);
                const char* name = sqlite3_column_name(pstmt, i);
                Value key(nullptr != name ? name : std::to_string(i).c_str(), doc.GetAllocator()); // copy string name
                
                if (SQLITE_INTEGER == type) {
                    rowValue.AddMember(key, sqlite3_column_int(pstmt, i), doc.GetAllocator());
                }else if (SQLITE_FLOAT == type) {
                    rowValue.AddMember(key, sqlite3_column_double(pstmt, i), doc.GetAllocator());
                }else if (SQLITE3_TEXT == type) {
                    Value value((const char*)sqlite3_column_text(pstmt, i), doc.GetAllocator()); // copy string name
                    rowValue.AddMember(key, value, doc.GetAllocator());
                }else if (SQLITE_NULL == type) {
                    rowValue.AddMember(key, Value().Move(), doc.GetAllocator());
                }else {
                    //todo++
                }
            }
            
            doc.PushBack(rowValue, doc.GetAllocator());
        }
        
        sqlite3_reset(pstmt);
        
        //to json string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        doc.Accept(writer);
        return buffer.GetString();
    }
    return "{}";
}





