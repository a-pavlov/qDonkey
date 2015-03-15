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
    static QString trayConnected()          {return prefix() + "tray/tray-connected.png";}
    static QString trayDisconnected()       {return prefix() + "tray/tray-disconnected.png";}
    static QString newMessage()             {return prefix() + "statusbar/MessagePending.ico";}

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

    // user resources
    static QString userUpdate()             {return usersPrefix() + "userupdate.ico";}
    static QString userDetails()            {return usersPrefix() + "UserDetails.ico";}
    static QString userMessage()            {return usersPrefix() + "UserMessage.ico";}
    static QString userAdd()                {return usersPrefix() + "UserAdd.ico";}
    static QString userDelete()             {return usersPrefix() + "UserDelete.ico";}
    static QString userFiles()              {return usersPrefix() + "UserFiles.ico";}
    static QString userFriend()             {return usersPrefix() + "Friend.ico";}
    static QString noUser()                 {return usersPrefix() + "client_red.ico";}
    static QString chat()                   {return usersPrefix() + "Chat.ico";}

    // TODO: recognize where the following icons are used and rename methods
    static QString user1()                  {return usersPrefix() + "Friend1.ico";}
    static QString user2()                  {return usersPrefix() + "Friend2.ico";}
    static QString user3()                  {return usersPrefix() + "Friend3.ico";}

    // actions / transfers
    static QString add()                    {return transfersPrefix() + "add.png";}
    static QString del()                    {return transfersPrefix() + "delete.png";}
    static QString play()                   {return transfersPrefix() + "play.png";}
    static QString pause()                  {return transfersPrefix() + "pause.png";}
    static QString upload()                 {return transfersPrefix() + "upload.png";}
    static QString download()               {return transfersPrefix() + "download.png";}
    static QString ratio()                  {return transfersPrefix() + "ratio.png";}
    static QString fileInfo()               {return transfersPrefix() + "FileInfo.png";}
    static QString allDownloads()           {return transfersPrefix() + "all-downloads.png";}
    static QString downloading()            {return transfersPrefix() + "downloading.png";}
    static QString uploading()              {return completed();}
    static QString completed()              {return transfersPrefix() + "completed.png";}
    static QString waiting()                {return transfersPrefix() + "waiting.png";}
    static QString activeDownloads()        {return transfersPrefix() + "filteractive.png";}
    static QString inactiveDownloads()      {return transfersPrefix() + "filterinactive.png";}
    static QString error()                  {return transfersPrefix() + "error.png";}
    static QString queuedDownloading()      {return pending();}
    static QString queuedUploading()        {return pending();}
    static QString pausedDownload()         {return transfersPrefix() + "downloading-pause.png";}
    static QString pausedUpload()           {return transfersPrefix() + "completed-pause.png";}
    static QString pending()                {return transfersPrefix() + "waiting-padded.png";}
    static QString stalledDownload()        {return pending();}
    static QString stalledUpload()          {return completed();}
    static QString checkingDownload()       {return pending();}
    static QString checkingUpload()         {return pending();}

    static QString openFolder()             {return actionsPrefix() + "folder_open.ico";}
    static QString shareFolder()            {return actionsPrefix() + "folder_share.ico";}
    static QString unshareFolder()          {return actionsPrefix() + "folder_unshare.ico";}
    static QString reloadFolder()           {return actionsPrefix() + "folder_reload.ico";}
};

#endif // RES_H
