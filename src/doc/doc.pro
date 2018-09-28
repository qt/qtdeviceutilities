TEMPLATE = aux

CONFIG += force_qt  # Needed for Qt include paths

QMAKE_DOCS = $$PWD/qtdeviceutilities.qdocconf

build_online_docs: QMAKE_DOCS_TARGETDIR = qtdeviceutilities

QMAKE_DOCS_OUTPUTDIR = $$OUT_PWD/qtdeviceutilities
