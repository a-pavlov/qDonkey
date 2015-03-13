#ifndef RES_H
#define RES_H

#include <QString>

class res
{
private:
    static QString prefix()             {return ":/icons/";}
    static QString actionsPrefix()      {return prefix() + "actions/";}
    static QString itemsPrefix()        {return prefix() + "items/";}
    static QString prefPrefix()         {return prefix() + "preferences/";}
    static QString searchPrefix()       {return prefix() + "search/";}
    static QString serverPrefix()       {return prefix() + "server/";}
    static QString statusbarPrefix()    {return prefix() + "statusbar/";}
    static QString toolbarPrefix()      {return prefix() + "toolbar/";}
    static QString transfersPrefix()    {return prefix() + "transfers/";}
    static QString usersPrefix()        {return prefix() + "users/";}

public:
    // 3-rd party icon packs
    static QString flag(const QString &name)        {return prefix() + "flags/" + name + ".png";}
    static QString oxygenIcon(const QString &id)    {return prefix() + "oxygen/" + id + ".png";}

    // items in lists and tables
    static QString itemAllFiles()           {return itemsPrefix() + "all.ico";}
    static QString itemAny()                {return itemsPrefix() + "any.png";}
    static QString itemArchive()            {return itemsPrefix() + "archive-file.png";}
    static QString itemCDImage()            {return itemsPrefix() + "cd-image-file.png";}
    static QString itemDocument()           {return itemsPrefix() + "document-file.png";}
    static QString itemEmuleCollection()    {return itemsPrefix() + "collection-file.png";}
    static QString itemFolder()             {return itemsPrefix() + "folder.png";}
    static QString itemMusic()              {return itemsPrefix() + "music-file.png";}
    static QString itemPicture()            {return itemsPrefix() + "picture-file.png";}
    static QString itemProgram()            {return itemsPrefix() + "program-file.png";}
    static QString itemVideo()              {return itemsPrefix() + "video-file.png";}
    static QString itemUser()               {return itemsPrefix() + "User.ico";}
    static QString itemTorrent()            {return itemsPrefix() + "torrent-file.png";}
    static QString itemED2KLink()           {return itemsPrefix() + "eD2kLink.png";}
    static QString itemMagnetLink()         {return itemsPrefix() + "magnet.png";}
    static QString itemHyperlink()          {return itemsPrefix() + "hyperlink.ico";}
    static QString sharedFolder()           {return favicon();}

    // common resources
    static QString favicon()                {return prefix() + "favicon.png";}
    static QString logo()                   {return prefix() + "splashScreen-emule060.jpg";}

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
    static QString toolbarConnected()       {return toolbarPrefix() + "emule_connected.png";}
    static QString toolbarConnecting()      {return toolbarPrefix() + "emule_connecting.png";}
    static QString toolbarDisconnected()    {return toolbarPrefix() + "emule_disconnected.png";}

    // statusbar resources
    static QString statusbarConnected()     {return statusbarPrefix() + "connectedHighHigh.png";}
    static QString statusbarDisconnected()  {return statusbarPrefix() + "connectedNotNot.png";}
    static QString statusbarGoogle()        {return statusbarPrefix() + "google_plus.png";}
    static QString message()                {return statusbarPrefix() + "Message.ico";}
    static QString messagePending()         {return statusbarPrefix() + "MessagePending.ico";}
    static QString noMessage()              {return statusbarPrefix() + "empty.ico";}

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
