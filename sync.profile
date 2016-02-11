%modules = ( # path to module name map
    "QtNetworkSettings" => "$basedir/src/networksettings",
    "QtBluetoothSettings" => "$basedir/src/bluetoothsettings",
    "QtLocaleSettings" => "$basedir/src/localesettings",
    "QtTimeDateSettings" => "$basedir/src/timedatesettings",
    "QtDisplaySettings" => "$basedir/src/displaysettings",
    "QtLocalDevice" => "$basedir/src/localdevice"
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
# Module dependencies.
# Every module that is required to build this module should have one entry.
# Each of the module version specifiers can take one of the following values:
#   - A specific Git revision.
#   - any git symbolic ref resolvable from the module's repository (e.g. "refs/heads/master" to track master branch)
#
%dependencies = (
        "qtbase" => "refs/heads/dev"
);
