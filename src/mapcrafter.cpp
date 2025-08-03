/*
 * Copyright 2012-2016 Moritz Hilscher
 *
 * This file is part of Mapcrafter.
 *
 * Mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mapcraftercore/config/loggingconfig.h"
#include "mapcraftercore/config/mapcrafterconfig.h"
#include "mapcraftercore/renderer/manager.h"
#include "mapcraftercore/util.h"
#include "mapcraftercore/version.h"

#include <iostream>
#include <string>
#include <cstring>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <locale.h>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

// evil, I know
using namespace mapcrafter;

int main(int argc, char** argv) {

	try {
		std::string locale = std::locale("").name();
	} catch (std::runtime_error& ex) {
		std::cerr << "There seems to be an issue with your locale, please verify your environment:"
			<< ex.what() << std::endl;
		throw ex;
	}

	renderer::RenderOpts opts;
	std::string arg_color, arg_config;
  std::string arg_template_dir, arg_texture_dir;

	po::options_description general("General options");
	general.add_options()
		("help,h", "shows this help message")
		("version,v", "shows the version of Mapcrafter")
		("mc-version", "shows the required Minecraft version")
    ("find-resources",    "shows available resource paths");

	po::options_description logging("Logging/output options");
	logging.add_options()
		("logging-config", po::value<fs::path>(&opts.logging_config),
			"the path to the global logging configuration file to use (automatically determined if not specified)")
		("color", po::value<std::string>(&arg_color)->default_value("auto"),
			"whether terminal output is colored (true, false or auto)")
		("batch,b", "deactivates the animated progress bar and enables the progress logger instead");

	po::options_description renderer("Renderer options");
	renderer.add_options()
		("config,c", po::value<std::string>(&arg_config),
			"the path to the configuration file to use (required)")
		("render-skip,s", po::value<std::vector<std::string>>(&opts.render_skip)->multitoken(),
			"skips rendering the specified map(s)")
		("render-reset,r", "skips rendering all maps")
		("render-auto,a", po::value<std::vector<std::string>>(&opts.render_auto)->multitoken(),
			"renders the specified map(s)")
		("render-force,f", po::value<std::vector<std::string>>(&opts.render_force)->multitoken(),
			"renders the specified map(s) completely")
		("render-force-all,F", "force renders all maps")
		("jobs,j", po::value<int>(&opts.jobs)->default_value(1),
			"the count of jobs to use when rendering the map")
    ("template-dir", po::value<std::string>(&arg_template_dir),
      "override the template directory")
    ("texture-dir", po::value<std::string>(&arg_texture_dir),
      "override the texture directory");

	po::options_description all("Allowed options");
	all.add(general).add(logging).add(renderer);

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, all), vm);
	} catch (po::error& ex) {
		std::cerr << "There is a problem parsing the command line arguments: "
				<< ex.what() << std::endl;
		std::cerr << "Use '" << argv[0] << " --help' for more information." << std::endl;
		return 1;
	}

	po::notify(vm);

	if (arg_color == "true")
		util::setcolor::setEnabled(util::TerminalColorStates::ENABLED);
	else if (arg_color == "false")
		util::setcolor::setEnabled(util::TerminalColorStates::DISABLED);
	else if (arg_color == "auto")
		util::setcolor::setEnabled(util::TerminalColorStates::AUTO);
	else {
		std::cerr << "Invalid argument '" << arg_color << "' for '--color'." << std::endl;
		std::cerr << "Allowed arguments are 'true', 'false' or 'auto'." << std::endl;
		std::cerr << "Use '" << argv[0] << " --help' for more information." << std::endl;
		return 1;
	}

	if (vm.count("help")) {
		std::cout << all << std::endl;
		std::cout << "Mapcrafter online documentation: <http://docs.mapcrafter.org>" << std::endl;
		return 0;
	}

	if (vm.count("version")) {
		std::cout << "Mapcrafter version: " << MAPCRAFTER_VERSION;
		if (strlen(MAPCRAFTER_GITVERSION))
			std::cout << " (" << MAPCRAFTER_GITVERSION << ")";
		std::cout << std::endl;
		return 0;
	}

	if (vm.count("mc-version")) {
		std::cout << MINECRAFT_VERSION << std::endl;
		return 0;
	}

	if (vm.count("find-resources")) {
    fs::path bin = util::findExecutablePath();
    std::cout << "Your home directory: " << util::findHomeDir().string() << "\n"
              << "Mapcrafter binary:  " << bin.string() << "\n";

    // if user passed --template-dir, use it; otherwise fall back to util::findTemplateDir()
    fs::path tpl = arg_template_dir.empty()
                   ? util::findTemplateDir()
                   : fs::path(arg_template_dir);
    fs::path tex = arg_texture_dir.empty()
                   ? util::findTextureDir()
                   : fs::path(arg_texture_dir);

    std::cout << "Template directory: " << tpl.string() << "\n"
              << "Texture directory:  " << tex.string() << "\n"
              << "Logging config:     " << opts.logging_config.string() << "\n";
    return 0;
  }

	if (!vm.count("config")) {
		std::cerr << "You have to specify a configuration file!" << std::endl;
		std::cerr << "Use '" << argv[0] << " --help' for more information." << std::endl;
		return 1;
	}

	opts.config = arg_config;
	opts.skip_all = vm.count("render-reset");
	opts.force_all = vm.count("render-force-all");
	opts.batch = vm.count("batch");
	if (!vm.count("logging-config"))
		opts.logging_config = util::findLoggingConfigFile();

	if (opts.skip_all && opts.force_all) {
		std::cerr << "You may only use one of --render-reset or --render-force-all!" << std::endl;
		std::cerr << "Use '" << argv[0] << " --help' for more information." << std::endl;
		return 1;
	}

	// ###
	// ### First big step: Load/parse/validate the configuration file
	// ###

	config::MapcrafterConfig config;
	config::ValidationMap validation = config.parseFile(opts.config.string());

	if (!arg_template_dir.empty()) config.overrideTemplateDir(fs::path(arg_template_dir));
	if (!arg_texture_dir.empty())  config.overrideTextureDir(fs::path(arg_texture_dir));

	
	// show infos/warnings/errors if configuration file has something
	if (!validation.isEmpty()) {
		if (validation.isCritical())
			LOG(FATAL) << "Unable to parse configuration file:";
		else
			LOG(WARNING) << "There is a problem parsing the configuration file:";
		validation.log();
		LOG(WARNING) << "Please have a look at the documentation.";
	}
	if (validation.isCritical())
		return 1;

	// parse global logging configuration file and configure logging
	config::LoggingConfig::configureLogging(opts.logging_config);

	// configure logging from this configuration file
	config.configureLogging();

	renderer::RenderManager manager(config);
	manager.setRenderBehaviors(renderer::RenderBehaviors::fromRenderOpts(config, opts));
	if (!manager.run(opts.jobs, opts.batch))
		return 1;
	return 0;
}
