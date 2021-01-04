#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::string>* loadDomains(const std::string &filename) {
    // Load the values from the file into a vector.
    std::ifstream domains_file(filename);

    if (domains_file.is_open()) {
        std::vector<std::string>* domains = new std::vector<std::string>();
        std::string domain;
        while (getline(domains_file, domain)) {
            // Strip whitespace
            const std::string whitespace = " \t\n\r";
            auto begin = domain.find_first_not_of(whitespace);
            auto end   = domain.find_last_not_of(whitespace);
            domain = domain.substr(begin, end);

            // If the first character in domain is #, or blank, then ignore
            if (!domain.empty() && domain[0] != '#') {
                domains->push_back(domain);
            }
        }

        // The vector will be held and not changed from now.
        domains->shrink_to_fit();
        return domains;
    } else {
        return nullptr;
    }
}
int main () {
    std::vector<std::string> *domains = loadDomains("adaway.org.txt");

    if (domains != nullptr) {
        std::cout << "Loaded " << domains->size() << " domains" << std::endl;
    } else {
        std::cout << "Failed to load domains" << std::endl;
    }
}