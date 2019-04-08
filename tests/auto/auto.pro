TEMPLATE = subdirs
SUBDIRS +=  qopcuaclient connection

QT_FOR_CONFIG += opcua-private

# only build declarative tests if at least one backend was built
qtHaveModule(qmltest):qtConfig(open62541)|qtConfig(uacpp) {
    SUBDIRS += declarative
}

# This tries to check if the server supports security
qtConfig(mbedtls): SUBDIRS += security
