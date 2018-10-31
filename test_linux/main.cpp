#include <iostream>
#include "../yaolog.h"
#include <unistd.h>
#define LOGID_I  "info"
#define LOGID_C  "custom"
#define LOGID_B  "bin"
#define LOGID_FB "formatted_bin"

int main()
{
    // call YAOLOG_INIT at the app entry point
    YAOLOG_INIT;

    // LOGID_I is a text logger
    // LOGID_C is a text logger
    // LOGID_B is a binary logger
    // LOGID_FB is a formatted binary logger
    YAOLOG_CREATE(LOGID_I, true, YaoUtil::LOG_TYPE_TEXT);
    YAOLOG_CREATE(LOGID_C, true, YaoUtil::LOG_TYPE_TEXT);
    YAOLOG_CREATE(LOGID_B, true, YaoUtil::LOG_TYPE_BIN);
    YAOLOG_CREATE(LOGID_FB, true, YaoUtil::LOG_TYPE_FORMATTED_BIN);

    LOG__(LOGID_I, "My name is %s! I'm %d!", "neil", 29);

    // use default log file path
    // it is "module file dir/log/logID_time.log"
    YAOLOG_SET_LOG_ATTR(
        LOGID_I, true,
        YaoUtil::OUT_FLAG_STDOUT | YaoUtil::OUT_FLAG_FILE,
        true, false, true, true, NULL);
    LOG__(LOGID_I, "My name is %s! I'm %d!", "neil", 29);

    // set from ini file
    // if parameter szINI is a filename like "logconfig.ini",
    // then it must be in module file dir
    // or you can use an absolute path like "/etc/logconfig.ini"
    YAOLOG_SET_ATTR_FROM_CONFIG_FILE(LOGID_C, "logconfig.ini");
    for (int i = 0; i < 100; i++)
    {
        if (i == 80)
        {
            // disable all logs...
            YAOLOG_DISABLE_ALL(true);
        }
        
        LOG__(LOGID_C, "You can modify the ini file and view effect(%d)!", i);
        usleep(1000);
    }

    // cancel disable
    YAOLOG_DISABLE_ALL(false);
    
    // output both to file and http server
    YAOLOG_SET_LOG_ATTR(
        LOGID_I, true, 
        YaoUtil::OUT_FLAG_FILE | YaoUtil::OUT_FLAG_REMOTE,
        true, true, true, true,
        "http://192.168.1.195/default.aspx");
    YAOLOG_SET_LOGFILE_ATTR(LOGID_I, false, false, false,
        "/tmp", "tt.log");
    LOG__(LOGID_I, "My name is %s! I'm %d!", "neil", 29);
    
    // log binary data
    YAOLOG_SET_ATTR_FROM_CONFIG_FILE(LOGID_B, "logconfig.ini");
    char bin[10] = { 0,1,2,3,4,5,6,7,8,9 };
    LOGBIN__(LOGID_B, bin, 10);

    // log formatted binary data
    YAOLOG_SET_ATTR_FROM_CONFIG_FILE(LOGID_FB, "logconfig.ini");
    char data1[20] = { 0x52,0x55,0x4F,0x4B,4,5,6,7,8,9,9,8,7,6,5,4,3,2,1,0 };
    char data2[4] = { 0x4F,0x4B,0x00,0x01 };
    LOGBIN_F__(LOGID_FB, "send", data1, 20);
    LOGBIN_F__(LOGID_FB, "recv", data2, 4);

    std::cin.get();

    // call YAOLOG_EXIT before app exit 
    YAOLOG_EXIT;

    return 0;
}
