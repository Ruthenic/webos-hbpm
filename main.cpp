#include "stb_ds.h"
#include "json.hpp"
#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <fstream>
#include <iostream>
using json = nlohmann::json;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    //code stolen from https://stackoverflow.com/questions/14985921/simple-way-to-download-a-file-with-curl
	size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int downloadFile(std::string url, char filename[]) {
	//code mostly stolen from https://stackoverflow.com/questions/14985921/simple-way-to-download-a-file-with-curl
	remove(filename);
	CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
	if (curl) {
	    fp = fopen(filename,"wb");
	    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	    res = curl_easy_perform(curl);
	    curl_easy_cleanup(curl);
	    fclose(fp);
	}
	return 0;
}

json downloadFileAsJSON(std::string url, char filename[]) {
	//code mostly stolen from https://stackoverflow.com/questions/14985921/simple-way-to-download-a-file-with-curl
	remove(filename);
	CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
	if (curl) {
	    fp = fopen(filename,"wb");
	    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	    res = curl_easy_perform(curl);
	    curl_easy_cleanup(curl);
	    fclose(fp);
	}
	std::ifstream manifest(filename);
	json manifestjson;
	manifest >> manifestjson;
	return manifestjson;
}

int main(int argc, char **argv) {
	json manifest = downloadFileAsJSON("https://www.ruthenic.com/hbpm-cli-repo/apps.json", "/tmp/repo.manifest.json");
	//hardcoding this for webOS atm
	std::string packageLocation = "/media/internal/bin";
	//printf(argv[1]);
	if (!strncmp(argv[1], "install", 7)) {
		//printf("User wants to install %s\n", argv[2]);
		//std::cout << manifest["packages"];
		for (auto& element: manifest["packages"]) {
			//std::cout << element << "\n";
			std::string title = element["title"];
			if (strcmp(title.c_str(), argv[2]) == 0) {
				std::cout << "Installing " << element["title"] << "\n";	
				json pkg_manifest = downloadFileAsJSON(element["manifestUrl"], "/tmp/pkg.manifest.json");
				std::string manifestUrl = element["manifestUrl"];
				std::string ipkUrl = pkg_manifest["ipkUrl"];
				std::string tmp = "/"; //i have no fucking idea why i have to do this but it works
				std::string packageUrl = manifestUrl.substr(0, manifestUrl.find_last_of("/")) + "/" + ipkUrl;
				downloadFile(packageUrl, (packageLocation + tmp + ipkUrl).c_str());
				std::string tmp2 = "chmod a+x ";
				system((tmp2 + (packageLocation + tmp + ipkUrl)).c_str());
				std::cout << "Installed " << element["title"] << "!\n";
			} else {
				//std::cout << "Not a match " << element["title"] << "\n";
			}
		}
	} else if (strcmp("list", argv[1]) == 0) {
		for (auto& element: manifest["packages"]) {
			std::string tmp = element["title"];
			tmp += "\n";
			std::cout << tmp.c_str() << "\n";
		}
	} else if (strcmp("remove", argv[1]) == 0) {
		for (auto& element: manifest["packages"]) {
			std::string title = element["title"];
			if (strcmp(title.c_str(), argv[2]) == 0) {
				std::cout << "Installing " << element["title"] << "\n";	
				json pkg_manifest = downloadFileAsJSON(element["manifestUrl"], "/tmp/pkg.manifest.json");
				std::string ipkUrl = pkg_manifest["ipkUrl"];
				std::string tmp = "/"; //i have no fucking idea why i have to do this but it works
				remove((packageLocation + tmp + ipkUrl).c_str());
			}
		}
	}
	return 0;
}
