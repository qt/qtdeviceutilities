TEMPLATE = aux
build_online_docs: {
    QMAKE_DOCS_TARGETDIR = b2qt
    QMAKE_DOCS = $$PWD/config/b2qt-online.qdocconf
} else {
    QMAKE_DOCS = $$PWD/config/b2qt.qdocconf
}

QMAKE_DOCS_OUTPUTDIR = $$OUT_PWD/b2qt
