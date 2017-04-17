
# Table of contents

- [Introduction](#introduction)
- [Compilation and Installation](#compilation-and-installation)
- [Demo 1: Simple Foo configuration](#demo1)
- [Demo 2: Town demo](#demo2)

# Introduction
﻿
This repository contains demo applications that uses [example libraries](https://gitlabe1.ext.net.nokia.com/cm/configurationmanagement-examples).

# Compilation and Installation
﻿
Above mentioned example libraries must be installed first.

Demo applications installation (in repository main level):

    ./autogen.sh
    ./configure
    make install

This will compile and install all demos from separate sub folders.

# Demo 1: Simple Foo configuration <a name="demo1"></a>

Implementation is in /cmdemo1/src/[main.cpp](cmdemo1/src/main.cpp) file.

Demo application uses CM `TransactionManager` API to start and end transactions
and `libcmexample1` library interface to access Foo configuration item.

Execution:

    cmdemo1

Demo executes few transactions in steps and output's progress to console.

## STEP 1: Write Foo configuration items

After this step (before continuing by pressing ENTER), you may output all keys
and written data with `loki` command:

    loki read-all --domain cmexample1domain --writer type=protobuf

Output contains also user friendly output for written values, this is enabled
by our `DataParsePlugin` implementation.

With following command you can check more information related to the transaction
we made (like execution times for our plugins):

    loki show-transaction-history

Check also following commands (see `loki --help` for more):

    loki show-db-stats
    loki show-transaction-global-stats

## STEP 2: Read Foo configuration items

Data written in previous step is read by starting a new read-only transaction.

## STEP 3: Read all descriptions

Reads all Foo descriptions with one request from configuration plugin.

## STEP 4: Cause validation failure

We had a validation rule that all Foo descriptions must be unique. Now we try
to create a new Foo configuration item, with same description text that was
given for first Foo item in first execution step.

When we try to commit, it will fail to "Validation failed" exception.

In syslog you will find log that was written by our validation plugin:

    cmpluginrunner-cmexample1domain[108154]: Validation failed. Foo description not unique: Description for first Foo.

## STEP 5: Delete Foo configuration items

Cleanup for Foo items created in previous steps.

# Demo 2: Town demo <a name="demo2"></a>

Demo consists of command line interface `cmdemo2cli` to configure town,
and daemon process `cmdemo2daemon` which will use town configuration.

When town configuration is modified daemon process gets notification
and re-reads town configuration.


## Command Line Interface

Usage instructions can be outputted by executing command without parameters:

    cmdemo2cli

Following commands will add one building and one citizen, show town configuration
and finally clean up everything:

    cmdemo2cli [addb Lib 10 1] [addc Jim 10 Lib]
    cmdemo2cli showb
    cmdemo2cli showc
    cmdemo2cli clear

If several commands are given in one command, then those are executed in one
transaction.

For example, above first command would not pass if given separately, this is
due the validation rules. Building cannot be created if there are no citizens
(tax payers), and citizen cannot be created if there are no buildings town.

## Daemon Process

Systemd service for `cmdemo2daemon` was created in installation phase, it
should be started:

    systemctl start cmdemo2daemon

If running demo in development environment, where CM services are not
managed by systemd, restart `cmactivator` with following commands:

    pkill cmactivator
    cmactivator -p

If `cmactivator` is managed by systemd (depends on deployment), then use systemd
commands instead:

    systemctl stop cmactivator
    systemctl start cmactivator

Activator is reading configuration from JSON files, which are delivered by
applications that wants to get notifications from certain domain configuration
changes.

In our demo this JSON file is found from [here](cmdemo2/cmdemo2daemonactivator.json).

In this JSON file activator is configured to listen "cmexample2notificationkey".
When change notification is published for this key, `systemctl reload cmdemo2daemon`
command is executed by activator.

Systemd reload command will send `SIGHUP` signal to `cmdemo2daemon` and it will
re-read town configuration. Daemon process will write system log every time it
reads town configuration.

To follow that notifications works as expected start journal log monitoring in
separate window:

    journalctl -f -u cmdemo2daemon

Then execute some `cmdemo2cli` commands that modifies Town configuration.

Above mentioned daemon log writings should be visible in journal monitoring.
