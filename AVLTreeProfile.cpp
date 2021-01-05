#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

// The files to load data from
const std::string domainFilesGroupA[] = {
    "adaway.org.txt",
    "adblock-nocoin-list.txt",
    "adguard-simplified.txt",
    "antipopads.txt",
    "anudeepnd-adservers.txt",
    "anudeepnd-coinminer.txt",
    "anudeepnd-facebook.txt",
    "digitalside-threat-intel.txt",
    "disconnect.me-ad.txt",
    "disconnect.me-malvertising.txt",
    "disconnect.me-tracking.txt",
    "easylist-ara.txt",
    "easylist-chn.txt",
    "easylist-deu.txt",
    "easylist-fra.txt",
    "easylist-ita.txt",
    "easylist-nld.txt",
    "easylist-rus.txt",
    "easylist-spa.txt",
    "easylist.txt",
    "easyprivacy.txt",
    "eth-phishing-detect.txt",
    "fademind-add.2o7net.txt",
    "fademind-add.dead.txt",
    "fademind-add.risk.txt",
    "fademind-add.spam.txt",
    "fanboy-annoyance.txt",
    "fanboy-social.txt"
};

const size_t domainFilesGroupASize = sizeof(domainFilesGroupA) / sizeof(domainFilesGroupA[0]);

const std::string domainFilesGroupB[] = {
    "gfrogeye-firstparty-trackers.txt",
    "hostsvn.txt",
    "kadhosts.txt",
    "kowabit.de-kwbtlist.txt",
    "lightswitch05-ads-and-tracking.txt",
    "malwaredomains.com-immortaldomains.txt",
    "malwaredomains.com-justdomains.txt",
    "matomo.org-spammers.txt",
    "mitchellkrogza-badd-boyz-hosts.txt",
    "molinero.dev.txt",
    "pgl.yoyo.org.txt",
    "phishing.army-extended.txt",
    "phishing.army.txt",
    "socram8888-notonmyshift.txt",
    "someonewhocares.org.txt",
    "spam404.com.txt",
    "stevenblack.txt",
    "stopforumspam.com.txt",
    "ublock-abuse.txt",
    "ublock-badware.txt",
    "ublock-privacy.txt",
    "ublock.txt",
    "urlhaus.txt",
    "winhelp2002.mvps.org.txt",
    "zerodot1-coinblockerlists-browser.txt",
    "zerodot1-coinblockerlists-optional.txt",
    "zerodot1-coinblockerlists.txt"
};

const size_t domainFilesGroupBSize = sizeof(domainFilesGroupB) / sizeof(domainFilesGroupB[0]);

static std::vector<std::string>* loadDomains(const std::string &filename) {
    // Load the values from the file into a vector.
    std::ifstream domains_file(filename);

    if (domains_file.is_open()) {
        auto *domains = new std::vector<std::string>();
        std::string domain;
        while (domains_file >> domain) {
            // If the first character in domain is #, or blank, then ignore
            if (!domain.empty() && domain[0] != '#') {
                domains->push_back(domain);
                domains_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        // The vector will be held and not changed from now.
        domains->shrink_to_fit();
        return domains;
    } else {
        return nullptr;
    }
}

// Big table all domains will end up in
std::vector<std::string>* groupA[domainFilesGroupASize];
std::vector<std::string>* groupB[domainFilesGroupBSize];

static void loadAllDomains() {
    for (unsigned int i = 0; i < domainFilesGroupASize; i++) {
        const std::string &domains_file = domainFilesGroupA[i];
        auto * const &domains = loadDomains(domains_file);

        groupA[i] = domains;

//        std::cout << domains_file;
//        if (domains != nullptr) {
//            std::cout << ": loaded " << domains->size() << " domains";
//        } else {
//            std::cout << ": failed to open";
//        }
//        std::cout << std::endl;
    }

    for (unsigned int i = 0; i < domainFilesGroupBSize; i++) {
        const std::string &domains_file = domainFilesGroupB[i];
        auto * const &domains = loadDomains(domains_file);

        groupB[i] = domains;

//        std::cout << domains_file;
//        if (domains != nullptr) {
//            std::cout << ": loaded " << domains->size() << " domains";
//        } else {
//            std::cout << ": failed to open";
//        }
//        std::cout << std::endl;
    }
}

int main () {
    std::cout << "Loading Domains" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    loadAllDomains();
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "Loaded all test files in " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;
}