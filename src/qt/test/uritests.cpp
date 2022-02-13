// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c)      2022 The CoinGreen Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uritests.h"

#include "guiutil.h"
#include "walletmodel.h"

#include <QUrl>
#include <iostream>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?req-dontexist="));
    QVERIFY(!GUIUtil::parseCoinGreenURI(uri, &rv));

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?dontexist="));
    QVERIFY(GUIUtil::parseCoinGreenURI(uri, &rv));
    QVERIFY(rv.address == QString("DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?label=Example Address"));
    QVERIFY(GUIUtil::parseCoinGreenURI(uri, &rv));
    QVERIFY(rv.address == QString("DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo"));
    QVERIFY(rv.label == QString("Example Address"));
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?amount=0.001"));
    QVERIFY(GUIUtil::parseCoinGreenURI(uri, &rv));
    QVERIFY(rv.address == QString("DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000);

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?amount=1.001"));
    QVERIFY(GUIUtil::parseCoinGreenURI(uri, &rv));
    QVERIFY(rv.address == QString("DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000);

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?amount=100&label=Example"));
    QVERIFY(GUIUtil::parseCoinGreenURI(uri, &rv));
    QVERIFY(rv.address == QString("DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("Example"));

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?message=Example Address"));
    QVERIFY(GUIUtil::parseCoinGreenURI(uri, &rv));
    QVERIFY(rv.address == QString("DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseCoinGreenURI("coingreen://DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?message=Example Address", &rv));
    QVERIFY(rv.address == QString("DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?req-message=Example Address"));
    QVERIFY(GUIUtil::parseCoinGreenURI(uri, &rv));

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?amount=1,000&label=Example"));
    QVERIFY(!GUIUtil::parseCoinGreenURI(uri, &rv));

    uri.setUrl(QString("coingreen:DDur8iJXFcct1ELDdmWLY8k7F83QBv6CXo?amount=1,000.0&label=Example"));
    QVERIFY(!GUIUtil::parseCoinGreenURI(uri, &rv));
}
