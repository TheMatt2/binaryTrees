#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

//#define AVLTREE_SANITYCHECK
#include "AVLTree.h"

// The files to load data from
const std::string domainFilesGroupA[] = {
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

const std::string domainFilesGroupB[] = {
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
        return false;
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

int main () {
    std::cout << "Loading Domains" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    loadAllDomains();
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "Loaded all test files in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;

    std::cout << "Loaded " << groupA.size() << " domains into groupA" << std::endl;
    std::cout << "Loaded " << groupB.size() << " domains into groupB" << std::endl;

    AVLTree<std::string> avlTree(compare);

    std::cout << "Add Group A into a tree" << std::endl;

    start = std::chrono::high_resolution_clock::now();
#ifdef AVLTREE_SANITYCHECK
    avlTree.sanityCheck();
#endif
    for (const std::string &domain: groupA) {
        avlTree.insert(domain);
#ifdef AVLTREE_SANITYCHECK
        avlTree.sanityCheck();
#endif
    }
    stop = std::chrono::high_resolution_clock::now();

    // Count values
    unsigned int count = 0;
    for (auto it = avlTree.preorder_begin(); it != avlTree.preorder_end(); it++) {
        count++;
    }

    std::cout << "Inserted " << count << " values in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;
    return 0;
    std::cout << "Removing all values from Group B" << std::endl;

    start = std::chrono::high_resolution_clock::now();
#ifdef AVLTREE_SANITYCHECK
    avlTree.sanityCheck();
#endif
    for (const std::string &domain: groupB) {
        avlTree.remove(domain);
#ifdef AVLTREE_SANITYCHECK
        avlTree.sanityCheck();
#endif
    }
    stop = std::chrono::high_resolution_clock::now();

    std::cout << "Removed all values in Group B in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;
}
