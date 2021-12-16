add_library(DeviceUtilities INTERFACE)
target_link_libraries(DeviceUtilities INTERFACE Qt::Core)

if(QT_BUILD_ONLINE_DOCS)
    set(DOC_CONF "online/qtdeviceutilities.qdocconf")
else()
    set(DOC_CONF "qtdeviceutilities.qdocconf")
endif()

qt_internal_add_docs(DeviceUtilities
    ${DOC_CONF}
)
