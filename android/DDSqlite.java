package com.cocos2d.diguo.template;

/**
 * Created by footprint on 7/5/17.
 */

public class DDSqlite {
    //打开数据库
    public native static boolean openDB(String path);

    //关闭当前打开的数据库
    public native static void closeDB();

    //执行sql语句
    public native static boolean executeSql(String sql);

    //生成准备条件语句
    public native static boolean prepareStatement(String key, String prepare);

    //绑定null值
    public native static boolean bindNull(String key, int index);

    //绑定int值
    public native static boolean bindInt(String key, int index, int value);

    //绑定double值
    public native static boolean bindDouble(String key, int index, double value);

    //绑定text值
    public native static boolean bindText(String key, int index, String text);

    //重置准备条件语句
    public native static boolean resetPrepare(String key);

    //执行准备条件语句并完成后重置
    public native static boolean exePrepareOnce(String key);

    //执行一次查询
    public native static String exePrepareQuery(String key);
}
