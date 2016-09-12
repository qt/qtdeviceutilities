TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS_TARGETDIR = qtdeviceutilities
    QMAKE_DOCS = $$PWD/config/qtdeviceutilities-online.qdocconf
} else {
    QMAKE_DOCS = $$PWD/config/qtdeviceutilities.qdocconf
}

QMAKE_DOCS_OUTPUTDIR = $$OUT_PWD/qtdeviceutilities
