// Copyright (c) 2011-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include <config/bitcoin-config.h>
#endif

#include <qt/paymentserver.h>

#include <qt/bitcoinunits.h>
#include <qt/guiutil.h>
#include <qt/optionsmodel.h>

#include <chainparams.h>
#include <interfaces/node.h>
#include <key_io.h>
#include <node/ui_interface.h>
#include <policy/policy.h>
#include <util/system.h>
#include <wallet/wallet.h>

#include <cstdlib>
#include <memory>

#include <QApplication>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFileOpenEvent>
#include <QHash>
#include <QList>
#include <QLocalServer>
#include <QLocalSocket>
#include <QStringList>
#include <QUrlQuery>

const int BITCOIN_IPC_CONNECT_TIMEOUT = 1000; // milliseconds
<<<<<<< HEAD
const QString BITCOIN_IPC_PREFIX("dogecoin:");
// BIP70 payment protocol messages
const char* BIP70_MESSAGE_PAYMENTACK = "PaymentACK";
const char* BIP70_MESSAGE_PAYMENTREQUEST = "PaymentRequest";
// BIP71 payment protocol media types
const char* BIP71_MIMETYPE_PAYMENT = "application/bitcoin-payment";
const char* BIP71_MIMETYPE_PAYMENTACK = "application/bitcoin-paymentack";
const char* BIP71_MIMETYPE_PAYMENTREQUEST = "application/bitcoin-paymentrequest";

struct X509StoreDeleter {
      void operator()(X509_STORE* b) {
          X509_STORE_free(b);
      }
};

struct X509Deleter {
      void operator()(X509* b) { X509_free(b); }
};

namespace // Anon namespace
{
    std::unique_ptr<X509_STORE, X509StoreDeleter> certStore;
}
=======
const QString BITCOIN_IPC_PREFIX("bitcoin:");
>>>>>>> 1.21-dev

//
// Create a name that is unique for:
//  testnet / non-testnet
//  data directory
//
static QString ipcServerName()
{
    QString name("DogecoinQt");

    // Append a simple hash of the datadir
    // Note that GetDataDir(true) returns a different path
    // for -testnet versus main net
    QString ddir(GUIUtil::boostPathToQString(GetDataDir(true)));
    name.append(QString::number(qHash(ddir)));

    return name;
}

//
// We store payment URIs and requests received before
// the main GUI window is up and ready to ask the user
// to send payment.

static QSet<QString> savedPaymentRequests;

//
// Sending to the server is done synchronously, at startup.
// If the server isn't already running, startup continues,
// and the items in savedPaymentRequest will be handled
// when uiReady() is called.
//
// Warning: ipcSendCommandLine() is called early in init,
// so don't use "Q_EMIT message()", but "QMessageBox::"!
//
void PaymentServer::ipcParseCommandLine(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        QString arg(argv[i]);
        if (arg.startsWith("-"))
            continue;

        // If the bitcoin: URI contains a payment request, we are not able to detect the
        // network as that would require fetching and parsing the payment request.
        // That means clicking such an URI which contains a testnet payment request
        // will start a mainnet instance and throw a "wrong network" error.
        if (arg.startsWith(BITCOIN_IPC_PREFIX, Qt::CaseInsensitive)) // bitcoin: URI
        {
            if (savedPaymentRequests.contains(arg)) continue;
            savedPaymentRequests.insert(arg);

            SendCoinsRecipient r;
            if (GUIUtil::parseBitcoinURI(arg, &r) && !r.address.isEmpty())
            {
                auto tempChainParams = CreateChainParams(gArgs, CBaseChainParams::MAIN);

                if (IsValidDestinationString(r.address.toStdString(), *tempChainParams)) {
                    SelectParams(CBaseChainParams::MAIN);
                } else {
                    tempChainParams = CreateChainParams(gArgs, CBaseChainParams::TESTNET);
                    if (IsValidDestinationString(r.address.toStdString(), *tempChainParams)) {
                        SelectParams(CBaseChainParams::TESTNET);
                    }
                }
            }
        }
<<<<<<< HEAD
        else if (QFile::exists(arg)) // Filename
        {
            savedPaymentRequests.append(arg);

            PaymentRequestPlus request;
            if (readPaymentRequestFromFile(arg, request))
            {
                if (request.getDetails().genesis() == "1a91e3dace36e2be3bf030a65679fe821aa1d6ef92e7c9902eb318182c355691")
                {
                    SelectParams(CBaseChainParams::MAIN);
                }
                else if (request.getDetails().genesis() == "bb0a78264637406b6360aad926284d544d7049f45189db5664f3c4d07350559e")
                {
                    SelectParams(CBaseChainParams::TESTNET);
                }
            }
        }
        else
        {
            // Printing to debug.log is about the best we can do here, the
            // GUI hasn't started yet so we can't pop up a message box.
            qWarning() << "PaymentServer::ipcSendCommandLine: Payment request file does not exist: " << arg;
        }
=======
>>>>>>> 1.21-dev
    }
}

//
// Sending to the server is done synchronously, at startup.
// If the server isn't already running, startup continues,
// and the items in savedPaymentRequest will be handled
// when uiReady() is called.
//
bool PaymentServer::ipcSendCommandLine()
{
    bool fResult = false;
    for (const QString& r : savedPaymentRequests)
    {
        QLocalSocket* socket = new QLocalSocket();
        socket->connectToServer(ipcServerName(), QIODevice::WriteOnly);
        if (!socket->waitForConnected(BITCOIN_IPC_CONNECT_TIMEOUT))
        {
            delete socket;
            socket = nullptr;
            return false;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out << r;
        out.device()->seek(0);

        socket->write(block);
        socket->flush();
        socket->waitForBytesWritten(BITCOIN_IPC_CONNECT_TIMEOUT);
        socket->disconnectFromServer();

        delete socket;
        socket = nullptr;
        fResult = true;
    }

    return fResult;
}

PaymentServer::PaymentServer(QObject* parent, bool startLocalServer) :
    QObject(parent),
    saveURIs(true),
    uriServer(nullptr),
    optionsModel(nullptr)
{
    // Install global event filter to catch QFileOpenEvents
    // on Mac: sent when you click bitcoin: links
    // other OSes: helpful when dealing with payment request files
    if (parent)
        parent->installEventFilter(this);

    QString name = ipcServerName();

    // Clean up old socket leftover from a crash:
    QLocalServer::removeServer(name);

    if (startLocalServer)
    {
        uriServer = new QLocalServer(this);

        if (!uriServer->listen(name)) {
            // constructor is called early in init, so don't use "Q_EMIT message()" here
<<<<<<< HEAD
            QMessageBox::critical(0, tr("Payment request error"),
                tr("Cannot start dogecoin: click-to-pay handler"));
=======
            QMessageBox::critical(nullptr, tr("Payment request error"),
                tr("Cannot start bitcoin: click-to-pay handler"));
>>>>>>> 1.21-dev
        }
        else {
            connect(uriServer, &QLocalServer::newConnection, this, &PaymentServer::handleURIConnection);
        }
    }
}

PaymentServer::~PaymentServer()
{
}

//
// OSX-specific way of handling bitcoin: URIs
//
bool PaymentServer::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent *fileEvent = static_cast<QFileOpenEvent*>(event);
        if (!fileEvent->file().isEmpty())
            handleURIOrFile(fileEvent->file());
        else if (!fileEvent->url().isEmpty())
            handleURIOrFile(fileEvent->url().toString());

        return true;
    }

    return QObject::eventFilter(object, event);
}

void PaymentServer::uiReady()
{
    saveURIs = false;
    for (const QString& s : savedPaymentRequests)
    {
        handleURIOrFile(s);
    }
    savedPaymentRequests.clear();
}

void PaymentServer::handleURIOrFile(const QString& s)
{
    if (saveURIs)
    {
        savedPaymentRequests.insert(s);
        return;
    }

    if (s.startsWith("bitcoin://", Qt::CaseInsensitive))
    {
        Q_EMIT message(tr("URI handling"), tr("'bitcoin://' is not a valid URI. Use 'bitcoin:' instead."),
            CClientUIInterface::MSG_ERROR);
    }
    else if (s.startsWith(BITCOIN_IPC_PREFIX, Qt::CaseInsensitive)) // bitcoin: URI
    {
        QUrlQuery uri((QUrl(s)));
        // normal URI
        {
            SendCoinsRecipient recipient;
            if (GUIUtil::parseBitcoinURI(s, &recipient))
            {
                if (!IsValidDestinationString(recipient.address.toStdString())) {
                    if (uri.hasQueryItem("r")) {  // payment request
                        Q_EMIT message(tr("URI handling"),
                            tr("Cannot process payment request because BIP70 is not supported.")+
                            tr("Due to widespread security flaws in BIP70 it's strongly recommended that any merchant instructions to switch wallets be ignored.")+
                            tr("If you are receiving this error you should request the merchant provide a BIP21 compatible URI."),
                            CClientUIInterface::ICON_WARNING);
                    }
                    Q_EMIT message(tr("URI handling"), tr("Invalid payment address"),
                        CClientUIInterface::MSG_ERROR);
                }
                else
                    Q_EMIT receivedPaymentRequest(recipient);
            }
            else
                Q_EMIT message(tr("URI handling"),
                    tr("URI cannot be parsed! This can be caused by an invalid Dogecoin address or malformed URI parameters."),
                    CClientUIInterface::ICON_WARNING);

            return;
        }
    }

    if (QFile::exists(s)) // payment request file
    {
        Q_EMIT message(tr("Payment request file handling"),
            tr("Cannot process payment request because BIP70 is not supported.")+
            tr("Due to widespread security flaws in BIP70 it's strongly recommended that any merchant instructions to switch wallets be ignored.")+
            tr("If you are receiving this error you should request the merchant provide a BIP21 compatible URI."),
            CClientUIInterface::ICON_WARNING);
    }
}

void PaymentServer::handleURIConnection()
{
    QLocalSocket *clientConnection = uriServer->nextPendingConnection();

    while (clientConnection->bytesAvailable() < (int)sizeof(quint32))
        clientConnection->waitForReadyRead();

    connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);

    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);
    if (clientConnection->bytesAvailable() < (int)sizeof(quint16)) {
        return;
    }
    QString msg;
    in >> msg;

    handleURIOrFile(msg);
}

<<<<<<< HEAD
//
// Warning: readPaymentRequestFromFile() is used in ipcSendCommandLine()
// so don't use "Q_EMIT message()", but "QMessageBox::"!
//
bool PaymentServer::readPaymentRequestFromFile(const QString& filename, PaymentRequestPlus& request)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << QString("PaymentServer::%1: Failed to open %2").arg(__func__).arg(filename);
        return false;
    }

    // BIP70 DoS protection
    if (!verifySize(f.size())) {
        return false;
    }

    QByteArray data = f.readAll();

    return request.parse(data);
}

bool PaymentServer::processPaymentRequest(const PaymentRequestPlus& request, SendCoinsRecipient& recipient)
{
    if (!optionsModel)
        return false;

    if (request.IsInitialized()) {
        // Payment request network matches client network?
        if (!verifyNetwork(request.getDetails())) {
            Q_EMIT message(tr("Payment request rejected"), tr("Payment request network doesn't match client network."),
                CClientUIInterface::MSG_ERROR);

            return false;
        }

        // Make sure any payment requests involved are still valid.
        // This is re-checked just before sending coins in WalletModel::sendCoins().
        if (verifyExpired(request.getDetails())) {
            Q_EMIT message(tr("Payment request rejected"), tr("Payment request expired."),
                CClientUIInterface::MSG_ERROR);

            return false;
        }
    } else {
        Q_EMIT message(tr("Payment request error"), tr("Payment request is not initialized."),
            CClientUIInterface::MSG_ERROR);

        return false;
    }

    recipient.paymentRequest = request;
    recipient.message = GUIUtil::HtmlEscape(request.getDetails().memo());

    request.getMerchant(certStore.get(), recipient.authenticatedMerchant);

    QList<std::pair<CScript, CAmount> > sendingTos = request.getPayTo();
    QStringList addresses;

    Q_FOREACH(const PAIRTYPE(CScript, CAmount)& sendingTo, sendingTos) {
        // Extract and check destination addresses
        CTxDestination dest;
        if (ExtractDestination(sendingTo.first, dest)) {
            // Append destination address
            addresses.append(QString::fromStdString(CBitcoinAddress(dest).ToString()));
        }
        else if (!recipient.authenticatedMerchant.isEmpty()) {
            // Unauthenticated payment requests to custom bitcoin addresses are not supported
            // (there is no good way to tell the user where they are paying in a way they'd
            // have a chance of understanding).
            Q_EMIT message(tr("Payment request rejected"),
                tr("Unverified payment requests to custom payment scripts are unsupported."),
                CClientUIInterface::MSG_ERROR);
            return false;
        }

        // Bitcoin amounts are stored as (optional) uint64 in the protobuf messages (see paymentrequest.proto),
        // but CAmount is defined as int64_t. Because of that we need to verify that amounts are in a valid range
        // and no overflow has happened.
        if (!verifyAmount(sendingTo.second)) {
            Q_EMIT message(tr("Payment request rejected"), tr("Invalid payment request."), CClientUIInterface::MSG_ERROR);
            return false;
        }

        // Extract and check amounts
        CTxOut txOut(sendingTo.second, sendingTo.first);
        if (txOut.IsDust(CWallet::discardThreshold)) {
            Q_EMIT message(tr("Payment request error"), tr("Requested payment amount of %1 is too small (below discard threshold).")
                .arg(BitcoinUnits::formatWithUnit(optionsModel->getDisplayUnit(), sendingTo.second)),
                CClientUIInterface::MSG_ERROR);

            return false;
        }

        recipient.amount += sendingTo.second;
        // Also verify that the final amount is still in a valid range after adding additional amounts.
        if (!verifyAmount(recipient.amount)) {
            Q_EMIT message(tr("Payment request rejected"), tr("Invalid payment request."), CClientUIInterface::MSG_ERROR);
            return false;
        }
    }
    // Store addresses and format them to fit nicely into the GUI
    recipient.address = addresses.join("<br />");

    if (!recipient.authenticatedMerchant.isEmpty()) {
        qDebug() << "PaymentServer::processPaymentRequest: Secure payment request from " << recipient.authenticatedMerchant;
    }
    else {
        qDebug() << "PaymentServer::processPaymentRequest: Insecure payment request to " << addresses.join(", ");
    }

    return true;
}

void PaymentServer::fetchRequest(const QUrl& url)
{
    QNetworkRequest netRequest;
    netRequest.setAttribute(QNetworkRequest::User, BIP70_MESSAGE_PAYMENTREQUEST);
    netRequest.setUrl(url);
    netRequest.setRawHeader("User-Agent", CLIENT_NAME.c_str());
    netRequest.setRawHeader("Accept", BIP71_MIMETYPE_PAYMENTREQUEST);
    netManager->get(netRequest);
}

void PaymentServer::fetchPaymentACK(CWallet* wallet, SendCoinsRecipient recipient, QByteArray transaction)
{
    const payments::PaymentDetails& details = recipient.paymentRequest.getDetails();
    if (!details.has_payment_url())
        return;

    QNetworkRequest netRequest;
    netRequest.setAttribute(QNetworkRequest::User, BIP70_MESSAGE_PAYMENTACK);
    netRequest.setUrl(QString::fromStdString(details.payment_url()));
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, BIP71_MIMETYPE_PAYMENT);
    netRequest.setRawHeader("User-Agent", CLIENT_NAME.c_str());
    netRequest.setRawHeader("Accept", BIP71_MIMETYPE_PAYMENTACK);

    payments::Payment payment;
    payment.set_merchant_data(details.merchant_data());
    payment.add_transactions(transaction.data(), transaction.size());

    // Create a new refund address, or re-use:
    QString account = tr("Refund from %1").arg(recipient.authenticatedMerchant);
    std::string strAccount = account.toStdString();
    std::set<CTxDestination> refundAddresses = wallet->GetAccountAddresses(strAccount);
    if (!refundAddresses.empty()) {
        CScript s = GetScriptForDestination(*refundAddresses.begin());
        payments::Output* refund_to = payment.add_refund_to();
        refund_to->set_script(&s[0], s.size());
    }
    else {
        CPubKey newKey;
        if (wallet->GetKeyFromPool(newKey)) {
            CKeyID keyID = newKey.GetID();
            wallet->SetAddressBook(keyID, strAccount, "refund");

            CScript s = GetScriptForDestination(keyID);
            payments::Output* refund_to = payment.add_refund_to();
            refund_to->set_script(&s[0], s.size());
        }
        else {
            // This should never happen, because sending coins should have
            // just unlocked the wallet and refilled the keypool.
            qWarning() << "PaymentServer::fetchPaymentACK: Error getting refund key, refund_to not set";
        }
    }

    int length = payment.ByteSize();
    netRequest.setHeader(QNetworkRequest::ContentLengthHeader, length);
    QByteArray serData(length, '\0');
    if (payment.SerializeToArray(serData.data(), length)) {
        netManager->post(netRequest, serData);
    }
    else {
        // This should never happen, either.
        qWarning() << "PaymentServer::fetchPaymentACK: Error serializing payment message";
    }
}

void PaymentServer::netRequestFinished(QNetworkReply* reply)
{
    reply->deleteLater();

    // BIP70 DoS protection
    if (!verifySize(reply->size())) {
        Q_EMIT message(tr("Payment request rejected"),
            tr("Payment request %1 is too large (%2 bytes, allowed %3 bytes).")
                .arg(reply->request().url().toString())
                .arg(reply->size())
                .arg(BIP70_MAX_PAYMENTREQUEST_SIZE),
            CClientUIInterface::MSG_ERROR);
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        QString msg = tr("Error communicating with %1: %2")
            .arg(reply->request().url().toString())
            .arg(reply->errorString());

        qWarning() << "PaymentServer::netRequestFinished: " << msg;
        Q_EMIT message(tr("Payment request error"), msg, CClientUIInterface::MSG_ERROR);
        return;
    }

    QByteArray data = reply->readAll();

    QString requestType = reply->request().attribute(QNetworkRequest::User).toString();
    if (requestType == BIP70_MESSAGE_PAYMENTREQUEST)
    {
        PaymentRequestPlus request;
        SendCoinsRecipient recipient;
        if (!request.parse(data))
        {
            qWarning() << "PaymentServer::netRequestFinished: Error parsing payment request";
            Q_EMIT message(tr("Payment request error"),
                tr("Payment request cannot be parsed!"),
                CClientUIInterface::MSG_ERROR);
        }
        else if (processPaymentRequest(request, recipient))
            Q_EMIT receivedPaymentRequest(recipient);

        return;
    }
    else if (requestType == BIP70_MESSAGE_PAYMENTACK)
    {
        payments::PaymentACK paymentACK;
        if (!paymentACK.ParseFromArray(data.data(), data.size()))
        {
            QString msg = tr("Bad response from server %1")
                .arg(reply->request().url().toString());

            qWarning() << "PaymentServer::netRequestFinished: " << msg;
            Q_EMIT message(tr("Payment request error"), msg, CClientUIInterface::MSG_ERROR);
        }
        else
        {
            Q_EMIT receivedPaymentACK(GUIUtil::HtmlEscape(paymentACK.memo()));
        }
    }
}

void PaymentServer::reportSslErrors(QNetworkReply* reply, const QList<QSslError> &errs)
{
    Q_UNUSED(reply);

    QString errString;
    Q_FOREACH (const QSslError& err, errs) {
        qWarning() << "PaymentServer::reportSslErrors: " << err;
        errString += err.errorString() + "\n";
    }
    Q_EMIT message(tr("Network request error"), errString, CClientUIInterface::MSG_ERROR);
}

=======
>>>>>>> 1.21-dev
void PaymentServer::setOptionsModel(OptionsModel *_optionsModel)
{
    this->optionsModel = _optionsModel;
}
<<<<<<< HEAD

void PaymentServer::handlePaymentACK(const QString& paymentACKMsg)
{
    // currently we don't further process or store the paymentACK message
    Q_EMIT message(tr("Payment acknowledged"), paymentACKMsg, CClientUIInterface::ICON_INFORMATION | CClientUIInterface::MODAL);
}

bool PaymentServer::verifyNetwork(const payments::PaymentDetails& requestDetails)
{
    Consensus::Params consensus = Params().GetConsensus(0);
    bool fVerified = requestDetails.genesis() == consensus.hashGenesisBlock.GetHex();
    if (!fVerified) {
        qWarning() << QString("PaymentServer::%1: Payment request network \"%2\" doesn't match client network \"%3\".")
            .arg(__func__)
            .arg(QString::fromStdString(requestDetails.genesis()))
            .arg(QString::fromStdString(consensus.hashGenesisBlock.GetHex()));
    }
    return fVerified;
}

bool PaymentServer::verifyExpired(const payments::PaymentDetails& requestDetails)
{
    bool fVerified = (requestDetails.has_expires() && (int64_t)requestDetails.expires() < GetTime());
    if (fVerified) {
        const QString requestExpires = QString::fromStdString(DateTimeStrFormat("%Y-%m-%d %H:%M:%S", (int64_t)requestDetails.expires()));
        qWarning() << QString("PaymentServer::%1: Payment request expired \"%2\".")
            .arg(__func__)
            .arg(requestExpires);
    }
    return fVerified;
}

bool PaymentServer::verifySize(qint64 requestSize)
{
    bool fVerified = (requestSize <= BIP70_MAX_PAYMENTREQUEST_SIZE);
    if (!fVerified) {
        qWarning() << QString("PaymentServer::%1: Payment request too large (%2 bytes, allowed %3 bytes).")
            .arg(__func__)
            .arg(requestSize)
            .arg(BIP70_MAX_PAYMENTREQUEST_SIZE);
    }
    return fVerified;
}

bool PaymentServer::verifyAmount(const CAmount& requestAmount)
{
    bool fVerified = MoneyRange(requestAmount);
    if (!fVerified) {
        qWarning() << QString("PaymentServer::%1: Payment request amount out of allowed range (%2, allowed 0 - %3).")
            .arg(__func__)
            .arg(requestAmount)
            .arg(MAX_MONEY);
    }
    return fVerified;
}

X509_STORE* PaymentServer::getCertStore()
{
    return certStore.get();
}
=======
>>>>>>> 1.21-dev
