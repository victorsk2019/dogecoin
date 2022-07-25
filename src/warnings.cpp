// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
#include "alert.h"
#include "sync.h"
#include "clientversion.h"
#include "uint256.h"
#include "util.h"
#include "utilstrencodings.h"
#include "warnings.h"

#include <boost/foreach.hpp>

CCriticalSection cs_warnings;
std::string strMiscWarning;
bool fLargeWorkForkFound = false;
bool fLargeWorkInvalidChainFound = false;
=======
#include <warnings.h>

#include <sync.h>
#include <util/string.h>
#include <util/system.h>
#include <util/translation.h>
>>>>>>> 1.21-dev

#include <vector>

static Mutex g_warnings_mutex;
static bilingual_str g_misc_warnings GUARDED_BY(g_warnings_mutex);
static bool fLargeWorkForkFound GUARDED_BY(g_warnings_mutex) = false;
static bool fLargeWorkInvalidChainFound GUARDED_BY(g_warnings_mutex) = false;

void SetMiscWarning(const bilingual_str& warning)
{
    LOCK(g_warnings_mutex);
    g_misc_warnings = warning;
}

void SetfLargeWorkForkFound(bool flag)
{
    LOCK(g_warnings_mutex);
    fLargeWorkForkFound = flag;
}

bool GetfLargeWorkForkFound()
{
    LOCK(g_warnings_mutex);
    return fLargeWorkForkFound;
}

void SetfLargeWorkInvalidChainFound(bool flag)
{
    LOCK(g_warnings_mutex);
    fLargeWorkInvalidChainFound = flag;
}

bilingual_str GetWarnings(bool verbose)
{
    bilingual_str warnings_concise;
    std::vector<bilingual_str> warnings_verbose;

<<<<<<< HEAD
std::string GetWarnings(const std::string& strFor)
{
    int nPriority = 0;
    std::string strStatusBar;
    std::string strRPC;
    std::string strGUI;
    const std::string uiAlertSeperator = "<hr />";

    LOCK(cs_warnings);
=======
    LOCK(g_warnings_mutex);
>>>>>>> 1.21-dev

    // Pre-release build warning
    if (!CLIENT_VERSION_IS_RELEASE) {
        warnings_concise = _("This is a pre-release test build - use at your own risk - do not use for mining or merchant applications");
        warnings_verbose.emplace_back(warnings_concise);
    }

    // Misc warnings like out of disk space and clock is wrong
<<<<<<< HEAD
    if (strMiscWarning != "")
    {
        nPriority = 1000;
        strStatusBar = strMiscWarning;
        strGUI += (strGUI.empty() ? "" : uiAlertSeperator) + strMiscWarning;
    }

    if (fLargeWorkForkFound)
    {
        nPriority = 2000;
        strStatusBar = strRPC = "Warning: The network does not appear to fully agree! Some miners appear to be experiencing issues.";
        strGUI += (strGUI.empty() ? "" : uiAlertSeperator) + _("Warning: The network does not appear to fully agree! Some miners appear to be experiencing issues.");
    }
    else if (fLargeWorkInvalidChainFound)
    {
        nPriority = 2000;
        strStatusBar = strRPC = "Warning: We do not appear to fully agree with our peers! You may need to upgrade, or other nodes may need to upgrade.";
        strGUI += (strGUI.empty() ? "" : uiAlertSeperator) + _("Warning: We do not appear to fully agree with our peers! You may need to upgrade, or other nodes may need to upgrade.");
    }

    // Alerts
    {
        LOCK(cs_mapAlerts);
        BOOST_FOREACH(PAIRTYPE(const uint256, CAlert)& item, mapAlerts)
        {
            const CAlert& alert = item.second;
            if (alert.AppliesToMe() && alert.nPriority > nPriority)
            {
                nPriority = alert.nPriority;
                strStatusBar = strGUI = alert.strStatusBar;
            }
        }
    }

    if (strFor == "gui")
        return strGUI;
    else if (strFor == "statusbar")
        return strStatusBar;
    else if (strFor == "rpc")
        return strRPC;
    assert(!"GetWarnings(): invalid parameter");
    return "error";
=======
    if (!g_misc_warnings.empty()) {
        warnings_concise = g_misc_warnings;
        warnings_verbose.emplace_back(warnings_concise);
    }

    if (fLargeWorkForkFound) {
        warnings_concise = _("Warning: The network does not appear to fully agree! Some miners appear to be experiencing issues.");
        warnings_verbose.emplace_back(warnings_concise);
    } else if (fLargeWorkInvalidChainFound) {
        warnings_concise = _("Warning: We do not appear to fully agree with our peers! You may need to upgrade, or other nodes may need to upgrade.");
        warnings_verbose.emplace_back(warnings_concise);
    }

    if (verbose) {
        return Join(warnings_verbose, Untranslated("<hr />"));
    }

    return warnings_concise;
>>>>>>> 1.21-dev
}
