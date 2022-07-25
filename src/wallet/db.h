// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_WALLET_DB_H
#define BITCOIN_WALLET_DB_H

#include <clientversion.h>
#include <fs.h>
#include <optional.h>
#include <streams.h>
#include <support/allocators/secure.h>
#include <util/memory.h>

#include <atomic>
#include <memory>
#include <string>

struct bilingual_str;

void SplitWalletPath(const fs::path& wallet_path, fs::path& env_directory, std::string& database_filename);

/** RAII class that provides access to a WalletDatabase */
class DatabaseBatch
{
private:
    virtual bool ReadKey(CDataStream&& key, CDataStream& value) = 0;
    virtual bool WriteKey(CDataStream&& key, CDataStream&& value, bool overwrite=true) = 0;
    virtual bool EraseKey(CDataStream&& key) = 0;
    virtual bool HasKey(CDataStream&& key) = 0;

public:
    explicit DatabaseBatch() {}
    virtual ~DatabaseBatch() {}

    DatabaseBatch(const DatabaseBatch&) = delete;
    DatabaseBatch& operator=(const DatabaseBatch&) = delete;

    virtual void Flush() = 0;
    virtual void Close() = 0;

    template <typename K, typename T>
    bool Read(const K& key, T& value)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(1000);
        ssKey << key;

<<<<<<< HEAD
        // Read
        Dbt datValue;
        datValue.set_flags(DB_DBT_MALLOC);
        int ret = pdb->get(activeTxn, &datKey, &datValue, 0);
        memory_cleanse(datKey.get_data(), datKey.get_size());
        bool success = false;
        if (datValue.get_data() != NULL) {
            // Unserialize value
            try {
                CDataStream ssValue((char*)datValue.get_data(), (char*)datValue.get_data() + datValue.get_size(), SER_DISK, CLIENT_VERSION);
                ssValue >> value;
                success = true;
            } catch (const std::exception&) {
                // In this case success remains 'false'
            }

            // Clear and free memory
            memory_cleanse(datValue.get_data(), datValue.get_size());
            free(datValue.get_data());
        }
        return ret == 0 && success;
=======
        CDataStream ssValue(SER_DISK, CLIENT_VERSION);
        if (!ReadKey(std::move(ssKey), ssValue)) return false;
        try {
            ssValue >> value;
            return true;
        } catch (const std::exception&) {
            return false;
        }
>>>>>>> 1.21-dev
    }

    template <typename K, typename T>
    bool Write(const K& key, const T& value, bool fOverwrite = true)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(1000);
        ssKey << key;

        CDataStream ssValue(SER_DISK, CLIENT_VERSION);
        ssValue.reserve(10000);
        ssValue << value;

<<<<<<< HEAD
        // Write
        int ret = pdb->put(activeTxn, &datKey, &datValue, (fOverwrite ? 0 : DB_NOOVERWRITE));

        // Clear memory in case it was a private key
        memory_cleanse(datKey.get_data(), datKey.get_size());
        memory_cleanse(datValue.get_data(), datValue.get_size());
        return (ret == 0);
=======
        return WriteKey(std::move(ssKey), std::move(ssValue), fOverwrite);
>>>>>>> 1.21-dev
    }

    template <typename K>
    bool Erase(const K& key)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(1000);
        ssKey << key;

<<<<<<< HEAD
        // Erase
        int ret = pdb->del(activeTxn, &datKey, 0);

        // Clear memory
        memory_cleanse(datKey.get_data(), datKey.get_size());
        return (ret == 0 || ret == DB_NOTFOUND);
=======
        return EraseKey(std::move(ssKey));
>>>>>>> 1.21-dev
    }

    template <typename K>
    bool Exists(const K& key)
    {
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(1000);
        ssKey << key;

<<<<<<< HEAD
        // Exists
        int ret = pdb->exists(activeTxn, &datKey, 0);

        // Clear memory
        memory_cleanse(datKey.get_data(), datKey.get_size());
        return (ret == 0);
    }

    Dbc* GetCursor()
    {
        if (!pdb)
            return NULL;
        Dbc* pcursor = NULL;
        int ret = pdb->cursor(NULL, &pcursor, 0);
        if (ret != 0)
            return NULL;
        return pcursor;
    }

    int ReadAtCursor(Dbc* pcursor, CDataStream& ssKey, CDataStream& ssValue, bool setRange = false)
    {
        // Read at cursor
        Dbt datKey;
        unsigned int fFlags = DB_NEXT;
        if (setRange) {
            datKey.set_data(ssKey.data());
            datKey.set_size(ssKey.size());
            fFlags = DB_SET_RANGE;
        }
        Dbt datValue;
        datKey.set_flags(DB_DBT_MALLOC);
        datValue.set_flags(DB_DBT_MALLOC);
        int ret = pcursor->get(&datKey, &datValue, fFlags);
        if (ret != 0)
            return ret;
        else if (datKey.get_data() == NULL || datValue.get_data() == NULL)
            return 99999;

        // Convert to streams
        ssKey.SetType(SER_DISK);
        ssKey.clear();
        ssKey.write((char*)datKey.get_data(), datKey.get_size());
        ssValue.SetType(SER_DISK);
        ssValue.clear();
        ssValue.write((char*)datValue.get_data(), datValue.get_size());

        // Clear and free memory
        memory_cleanse(datKey.get_data(), datKey.get_size());
        memory_cleanse(datValue.get_data(), datValue.get_size());
        free(datKey.get_data());
        free(datValue.get_data());
        return 0;
    }

public:
    bool TxnBegin()
    {
        if (!pdb || activeTxn)
            return false;
        DbTxn* ptxn = bitdb.TxnBegin();
        if (!ptxn)
            return false;
        activeTxn = ptxn;
        return true;
    }

    bool TxnCommit()
    {
        if (!pdb || !activeTxn)
            return false;
        int ret = activeTxn->commit(0);
        activeTxn = NULL;
        return (ret == 0);
    }

    bool TxnAbort()
    {
        if (!pdb || !activeTxn)
            return false;
        int ret = activeTxn->abort();
        activeTxn = NULL;
        return (ret == 0);
    }

    bool ReadVersion(int& nVersion)
    {
        nVersion = 0;
        return Read(std::string("version"), nVersion);
    }

    bool WriteVersion(int nVersion)
    {
        return Write(std::string("version"), nVersion);
    }

    bool static Rewrite(const std::string& strFile, const char* pszSkip = NULL);
=======
        return HasKey(std::move(ssKey));
    }

    virtual bool StartCursor() = 0;
    virtual bool ReadAtCursor(CDataStream& ssKey, CDataStream& ssValue, bool& complete) = 0;
    virtual void CloseCursor() = 0;
    virtual bool TxnBegin() = 0;
    virtual bool TxnCommit() = 0;
    virtual bool TxnAbort() = 0;
>>>>>>> 1.21-dev
};

/** An instance of this class represents one database.
 **/
class WalletDatabase
{
public:
    /** Create dummy DB handle */
    WalletDatabase() : nUpdateCounter(0), nLastSeen(0), nLastFlushed(0), nLastWalletUpdate(0) {}
    virtual ~WalletDatabase() {};

    /** Open the database if it is not already opened. */
    virtual void Open() = 0;

    //! Counts the number of active database users to be sure that the database is not closed while someone is using it
    std::atomic<int> m_refcount{0};
    /** Indicate the a new database user has began using the database. Increments m_refcount */
    virtual void AddRef() = 0;
    /** Indicate that database user has stopped using the database and that it could be flushed or closed. Decrement m_refcount */
    virtual void RemoveRef() = 0;

    /** Rewrite the entire database on disk, with the exception of key pszSkip if non-zero
     */
    virtual bool Rewrite(const char* pszSkip=nullptr) = 0;

    /** Back up the entire database to a file.
     */
    virtual bool Backup(const std::string& strDest) const = 0;

    /** Make sure all changes are flushed to database file.
     */
    virtual void Flush() = 0;
    /** Flush to the database file and close the database.
     *  Also close the environment if no other databases are open in it.
     */
    virtual void Close() = 0;
    /* flush the wallet passively (TRY_LOCK)
       ideal to be called periodically */
    virtual bool PeriodicFlush() = 0;

    virtual void IncrementUpdateCounter() = 0;

    virtual void ReloadDbEnv() = 0;

    /** Return path to main database file for logs and error messages. */
    virtual std::string Filename() = 0;

    virtual std::string Format() = 0;

    std::atomic<unsigned int> nUpdateCounter;
    unsigned int nLastSeen;
    unsigned int nLastFlushed;
    int64_t nLastWalletUpdate;

    /** Make a DatabaseBatch connected to this database */
    virtual std::unique_ptr<DatabaseBatch> MakeBatch(bool flush_on_close = true) = 0;
};

/** RAII class that provides access to a DummyDatabase. Never fails. */
class DummyBatch : public DatabaseBatch
{
private:
    bool ReadKey(CDataStream&& key, CDataStream& value) override { return true; }
    bool WriteKey(CDataStream&& key, CDataStream&& value, bool overwrite=true) override { return true; }
    bool EraseKey(CDataStream&& key) override { return true; }
    bool HasKey(CDataStream&& key) override { return true; }

public:
    void Flush() override {}
    void Close() override {}

    bool StartCursor() override { return true; }
    bool ReadAtCursor(CDataStream& ssKey, CDataStream& ssValue, bool& complete) override { return true; }
    void CloseCursor() override {}
    bool TxnBegin() override { return true; }
    bool TxnCommit() override { return true; }
    bool TxnAbort() override { return true; }
};

/** A dummy WalletDatabase that does nothing and never fails. Only used by unit tests.
 **/
class DummyDatabase : public WalletDatabase
{
public:
    void Open() override {};
    void AddRef() override {}
    void RemoveRef() override {}
    bool Rewrite(const char* pszSkip=nullptr) override { return true; }
    bool Backup(const std::string& strDest) const override { return true; }
    void Close() override {}
    void Flush() override {}
    bool PeriodicFlush() override { return true; }
    void IncrementUpdateCounter() override { ++nUpdateCounter; }
    void ReloadDbEnv() override {}
    std::string Filename() override { return "dummy"; }
    std::string Format() override { return "dummy"; }
    std::unique_ptr<DatabaseBatch> MakeBatch(bool flush_on_close = true) override { return MakeUnique<DummyBatch>(); }
};

enum class DatabaseFormat {
    BERKELEY,
    SQLITE,
};

struct DatabaseOptions {
    bool require_existing = false;
    bool require_create = false;
    Optional<DatabaseFormat> require_format;
    uint64_t create_flags = 0;
    SecureString create_passphrase;
    bool verify = true;
};

enum class DatabaseStatus {
    SUCCESS,
    FAILED_BAD_PATH,
    FAILED_BAD_FORMAT,
    FAILED_ALREADY_LOADED,
    FAILED_ALREADY_EXISTS,
    FAILED_NOT_FOUND,
    FAILED_CREATE,
    FAILED_LOAD,
    FAILED_VERIFY,
    FAILED_ENCRYPT,
};

std::unique_ptr<WalletDatabase> MakeDatabase(const fs::path& path, const DatabaseOptions& options, DatabaseStatus& status, bilingual_str& error);

#endif // BITCOIN_WALLET_DB_H
