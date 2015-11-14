unix:!android {
    isEmpty(target.path) {
        qnx {
            target.path = /tmp/$${TARGET}/bin
        } else {
            target.path = /opt/$${TARGET}/bin
        }
        export(target.path)
    }
    INSTALLS += target
}

#deployment.files += server.met
#deployment.path = /

#logo_ldpi.files += images/ldpi/logo.png
#logo_ldpi.path = /res/drawable-ldpi/

#logo_mdpi.files += images/mdpi/logo.png
#logo_mdpi.path = /res/drawable-mdpi/

#logo_hdpi.files += images/hdpi/logo.png
#logo_hdpi.path = /res/drawable-hdpi/

#INSTALLS += deployment logo_ldpi logo_mdpi logo_hdpi

export(INSTALLS)

