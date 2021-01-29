#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#define AVLTREE_SANITY_CHECK
#define AVLTREE_EXTENDED_SANITY_CHECK
#include "AVLTree.h"

// The files to load data from
const std::string domainFilesGroupA[] = {
    "data/hostsvn.txt",
    "data/kadhosts.txt",
    "data/kowabit.de-kwbtlist.txt",
    "data/lightswitch05-ads-and-tracking.txt",
    "data/malwaredomains.com-immortaldomains.txt",
    "data/malwaredomains.com-justdomains.txt",
    "data/matomo.org-spammers.txt",
    "data/mitchellkrogza-badd-boyz-hosts.txt",
    "data/molinero.dev.txt",
    "data/pgl.yoyo.org.txt",
    "data/phishing.army-extended.txt",
    "data/phishing.army.txt",
    "data/socram8888-notonmyshift.txt",
    "data/someonewhocares.org.txt",
    "data/spam404.com.txt",
    "data/stevenblack.txt",
    "data/stopforumspam.com.txt",
};

const std::string domainFilesGroupB[] = {
    "data/gfrogeye-firstparty-trackers.txt",
    "data/adaway.org.txt",
    "data/adblock-nocoin-list.txt",
    "data/adguard-simplified.txt",
    "data/antipopads.txt",
    "data/anudeepnd-adservers.txt",
    "data/anudeepnd-coinminer.txt",
    "data/anudeepnd-facebook.txt",
    "data/digitalside-threat-intel.txt",
    "data/disconnect.me-ad.txt",
    "data/disconnect.me-malvertising.txt",
    "data/disconnect.me-tracking.txt",
    "data/easylist-ara.txt",
    "data/easylist-chn.txt",
    "data/easylist-deu.txt",
    "data/easylist-fra.txt",
    "data/easylist-ita.txt",
    "data/easylist-nld.txt",
    "data/easylist-rus.txt",
    "data/easylist-spa.txt",
    "data/easylist.txt",
    "data/easyprivacy.txt",
    "data/eth-phishing-detect.txt",
    "data/fademind-add.2o7net.txt",
    "data/fademind-add.dead.txt",
    "data/fademind-add.risk.txt",
    "data/fademind-add.spam.txt",
    "data/fanboy-annoyance.txt",
    "data/fanboy-social.txt",
    "data/ublock-abuse.txt",
    "data/ublock-badware.txt",
    "data/ublock-privacy.txt",
    "data/ublock.txt",
    "data/urlhaus.txt",
    "data/winhelp2002.mvps.org.txt",
    "data/zerodot1-coinblockerlists-browser.txt",
    "data/zerodot1-coinblockerlists-optional.txt",
    "data/zerodot1-coinblockerlists.txt"
};

static bool loadDomains(const std::string &filename, std::vector<std::string> &domains) {
    // Load the values from the file into a vector.
    std::ifstream domains_file(filename);

    if (domains_file.is_open()) {
        std::string domain;
        while (domains_file >> domain) {
            // If the first character in domain is #, or blank, then ignore
            if (!domain.empty() && domain[0] != '#') {
                domains.push_back(domain);
                domains_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        return true;
    } else {
        // Failed to load file.
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }
}

// Big table all domains will end up in
std::vector<std::string> groupA;
std::vector<std::string> groupB;

static void loadAllDomains() {
    for (const std::string &domains_file: domainFilesGroupA) {
        loadDomains(domains_file, groupA);
    }
    groupA.shrink_to_fit();

    for (const std::string &domains_file: domainFilesGroupB) {
        loadDomains(domains_file, groupB);
    }
    groupB.shrink_to_fit();
}

// Set up for tree, a compare function
int compare(const std::string &a, const std::string &b) {
    return a.compare(b);
}

int main() {
    std::cout << "Loading Domains" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    loadAllDomains();
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "Loaded all test files in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << "ms" << std::endl;

    std::cout << "Loaded " << groupA.size() << " domains into Group A" << std::endl;
    std::cout << "Loaded " << groupB.size() << " domains into Group B" << std::endl;

    AVLTreeCountable<std::string> avlTree = AVLTreeCountable<std::string>(compare);

    std::cout << "Add Group A into a tree" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (auto &domain: groupA) {
#ifdef AVLTREE_EXTENDED_SANITY_CHECK
        avlTree.sanityCheck();
#endif
        avlTree.insert(domain);
    }
#ifdef AVLTREE_SANITY_CHECK
    avlTree.sanityCheck();
#endif
    stop = std::chrono::high_resolution_clock::now();

    // Count values
    unsigned int count = avlTree.count();

    std::cout << "Inserted " << count << " values in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << "ms" << std::endl;

    std::cout << "Removing " << groupB.size() << " values from Group B" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (auto &domain: groupB) {
#ifdef AVLTREE_EXTENDED_SANITY_CHECK
        avlTree.sanityCheck();
#endif
        count -= avlTree.remove(domain);
    }
#ifdef AVLTREE_SANITY_CHECK
    avlTree.sanityCheck();
#endif
    stop = std::chrono::high_resolution_clock::now();

    // Check that counts match
    count -= avlTree.count();

    if (count != 0) {
        // Something failed
        std::cerr << "AVLTree changed size unexpectedly during item removal." << std::endl;
        exit(2);
    }

    std::cout << "Removed all values in Group B in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << "ms" << std::endl;
}
