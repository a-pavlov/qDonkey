#ifndef RES_H
#define RES_H

#include <QString>

class res
{
private:
    static QString prefix()             {return ":/icons/";}
    static QString actionsPrefix()      {return prefix() + "actions/";}
    static QString itemsPrefix()        {return prefix() + "common/";}
    static QString prefPrefix()         {return prefix() + "preferences/";}
    static QString searchPrefix()       {return prefix() + "search/";}
    static QString serverPrefix()       {return prefix() + "server/";}
    static QString statusbarPrefix()    {return prefix() + "statusbar/";}
    static QString toolbarPrefix()      {return prefix(); }
    static QString transfersPrefix()    {return prefix() + "transfers/";}
    static QString usersPrefix()        {return prefix() + "users/";}

public:
    // 3-rd party icon packs
    static QString flag(const QString &name)        {return prefix() + "flags/" + name + ".png";}
    static QString oxygenIcon(const QString &id)    {return prefix() + "oxygen/" + id + ".png";}

    static QString itemAny()                {return itemsPrefix() + "FileTypeAny.ico";}
    static QString itemArchive()            {return itemsPrefix() + "FileTypeArchive.ico";}
    static QString itemCDImage()            {return itemsPrefix() + "FileTypeCDImage.ico";}
    static QString itemDocument()           {return itemsPrefix() + "FileTypeDocument.ico";}
    static QString itemEmuleCollection()    {return itemsPrefix() + "FileTypeEmuleCollection.ico";}
    static QString itemMusic()              {return itemsPrefix() + "FileTypeAudio.ico";}
    static QString itemPicture()            {return itemsPrefix() + "FileTypePicture.ico";}
    static QString itemProgram()            {return itemsPrefix() + "FileTypeProgram.ico";}
    static QString itemVideo()              {return itemsPrefix() + "FileTypeVideo.ico";}
    static QString user()                   {return itemsPrefix() + "User.ico"; }

    // common resources
    static QString favicon()                {return prefix() + "newmule.png";}

    // preferences dialog resources
    static QString prefAdvanced()           {return prefPrefix() + "advanced.png";}
    static QString prefBehavior()           {return prefPrefix() + "behavior.png";}
    static QString prefBitTorrent()         {return prefPrefix() + "bitTorrent.png";}
    static QString prefConnections()        {return prefPrefix() + "connections.png";}
    static QString prefDownloads()          {return prefPrefix() + "downloads.png";}
    static QString prefEDonkey()            {return prefPrefix() + "eMule.png";}
    static QString prefSpeed()              {return prefPrefix() + "speed.png";}
    static QString prefRemote()             {return prefPrefix() + "emule-remote-icon.png";}

    // system tray resources
    static QString trayConnected()          {return prefix() + "TrayConnected.ico";}
    static QString trayDisconnected()       {return prefix() + "TrayDisconnected.ico";}

    // toolbar resources
    static QString toolbarConnected()       {return toolbarPrefix() + "ConnectDrop.png";}
    static QString toolbarConnecting()      {return toolbarPrefix() + "ConnectStop.png";}
    static QString toolbarDisconnected()    {return toolbarPrefix() + "ConnectDoBig.png";}

    // statusbar resources
    static QString statusUD00()             {return statusbarPrefix() + "Up0down0.ico";}
    static QString statusUD01()             {return statusbarPrefix() + "Up0down1.ico";}
    static QString statusUD10()             {return statusbarPrefix() + "Up1down0.ico";}
    static QString statusUD11()             {return statusbarPrefix() + "Up1down1.ico";}
    static QString statusConnected()        {return statusbarPrefix() + "connected.ico";}
    static QString statusDisconnected()     {return statusbarPrefix() + "disconnected.ico";}

    // servers resources
    static QString connect()                {return serverPrefix() + "ConnectDo.ico";}
    static QString deleteAll()              {return serverPrefix() + "DeleteAll.ico";}
    static QString deleteSelected()         {return serverPrefix() + "DeleteSelected.ico";}

    // search resources
    static QString searchArchive()          {return searchPrefix() + "SearchArchive.png";}
    static QString searchResults()          {return searchPrefix() + "search-results.png";}
    static QString searchResult()           {return searchPrefix() + "SearchResult.png";}
    static QString searchRelated()          {return searchPrefix() + "SearchRelated.png";}
    static QString searchFilter()           {return searchPrefix() + "SearchFilter.png";}
    static QString clear()                  {return searchPrefix() + "clear1.ico";}
    static QString preview()                {return searchPrefix() + "preview32.png";}
    static QString searchInProgress()       {return searchPrefix() + "preloader-emule060.GIF";}
    static QString searchSucceeded()        {return searchPrefix() + "completed-search.png";}
    static QString searchFailed()           {return searchPrefix() + "error-search.png";}

    static QString searchActive()           {return searchPrefix() + "SearchActive.png"; }

    static QString fileOnDisk()             {return prefix() + "files/HardDisk.ico"; }
};

#endif // RES_H
