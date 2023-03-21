#include "TestManager.hpp"
#include "MinecraftServer.hpp"

#include <catch2/catch_test_case_info.hpp>

#include <protocolCraft/Types/NBT/NBT.hpp>

#include <botcraft/Network/NetworkManager.hpp>

#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>

std::string ReplaceCharacters(const std::string& in, const std::vector<std::pair<char, std::string>>& replacements = { {'"', "\\\""}, {'\n', "\\n"} });

TestManager::TestManager()
{
	current_offset = {spacing_x, 2, 2 * spacing_z };
	current_test_index = 0;
	bot_index = 0;
	chunk_loader_id = -1;
}

TestManager::~TestManager()
{

}

TestManager& TestManager::GetInstance()
{
	static TestManager instance;
	return instance;
}

const Botcraft::Position& TestManager::GetCurrentOffset() const
{
	return current_offset;
}


#if PROTOCOL_VERSION == 340
void TestManager::SetBlock(const std::string& name, const Botcraft::Position& pos, const int block_variant, const std::map<std::string, std::string>& metadata) const
{
	MakeSureLoaded(pos);
	std::stringstream command;
	command
		<< "setblock" << " "
		<< pos.x << " "
		<< pos.y << " "
		<< pos.z << " "
		<< ((name.size() > 10 && name.substr(0, 10) == "minecraft:") ? "" : "minecraft:") << name << " "
		<< block_variant << " "
		<< "replace";
	if (!metadata.empty())
	{
		command << " {";
		int index = 0;
		for (const auto& [k, v] : metadata)
		{
			command << k << ":";
			if (v.find(' ') != std::string::npos ||
				v.find(',') != std::string::npos ||
				v.find(':') != std::string::npos)
			{
				// Make sure the whole string is between quotes and all internal quotes are escaped
				command << "\"" << ReplaceCharacters(v) << "\"";
			}
			else
			{
				command << v;
			}
			command << (index == metadata.size() - 1 ? "" : ",");
			index += 1;
		}
		command << "}";
	}
	MinecraftServer::GetInstance().SendLine(command.str());
	MinecraftServer::GetInstance().WaitLine(".*: Block placed.*", 2000);
}
#endif

void TestManager::CreateBook(const Botcraft::Position& pos, const std::vector<std::string>& pages, const std::string& facing, const std::string& title, const std::string& author, const std::vector<std::string>& description)
{
	int facing_id = 0;
	if (facing == "south")
	{
		facing_id = 0;
	}
	else if (facing == "west")
	{
		facing_id = 1;
	}
	else if (facing == "north")
	{
		facing_id = 2;
	}
	else if (facing == "east")
	{
		facing_id = 3;
	}

	std::stringstream command;

	command
		<< "summon" << " "
		<< "minecraft:item_frame" << " "
		<< pos.x << " "
		<< pos.y << " "
		<< pos.z << " "
		<< "{Facing:" << facing_id << ","
		<< "Item:{"
			<< "id:\"written_book\"" << ","
			<< "Count:1" << ","
			<< "tag:{"
				<< "pages:[";
	for (size_t i = 0; i < pages.size(); ++i)
	{
		command
			<< "\"{"
			<< "\\\"text\\\"" << ":" << "\\\"" << ReplaceCharacters(pages[i], { {'\n', "\\\\n"}, {'"', "\\\""} }) << "\\\""
			<< "}\"" << ((i < pages.size() - 1) ? "," : "");
	}
	command << "]";
	if (!title.empty())
	{
		command << ","
			<< "title" << ":" << "\"" << ReplaceCharacters(title) << "\"";
	}
	if (!author.empty())
	{
		command << ","
			<< "author" << ":" << "\"" << ReplaceCharacters(author) << "\"";
	}
	if (!description.empty())
	{
		command << "," << "display" << ":"
			<< "{Lore:[";
		for (size_t i = 0; i < description.size(); ++i)
		{
			command
				<< "\""
				<< ReplaceCharacters(description[i])
				<< "\"" << ((i < description.size() - 1) ? "," : "");
		}
		command << "]}";
	}
	command
		<< "}" // tag
		<< "}" // Item
		<< "}"; // Main

	MinecraftServer::GetInstance().SendLine(command.str());
	MinecraftServer::GetInstance().WaitLine(".*?: Object successfully summoned.*", 2000);
}

void TestManager::SetGameMode(const std::string& name, const Botcraft::GameType gamemode) const
{
	std::string gamemode_string;
	switch (gamemode)
	{
	case Botcraft::GameType::Survival:
		gamemode_string = "survival";
		break;
	case Botcraft::GameType::Creative:
		gamemode_string = "creative";
		break;
	case Botcraft::GameType::Adventure:
		gamemode_string = "adventure";
		break;
	case Botcraft::GameType::Spectator:
		gamemode_string = "spectator";
		break;
	default:
		return;
	}
	std::stringstream command;
	command
		<< "gamemode" << " "
		<< gamemode_string << " "
		<< name;
	MinecraftServer::GetInstance().SendLine(command.str());
	MinecraftServer::GetInstance().WaitLine(".*? Set " + name + "'s game mode to.*", 2000);
}

void TestManager::Teleport(const std::string& name, const Botcraft::Vector3<double>& pos) const
{
	std::stringstream command;
	command
		<< "teleport" << " "
		<< name << " "
		<< pos.x << " "
		<< pos.y << " "
		<< pos.z << " ";
	MinecraftServer::GetInstance().SendLine(command.str());
	MinecraftServer::GetInstance().WaitLine(".*?Teleported " + name + " to.*", 2000);
}

Botcraft::Position TestManager::GetStructureSize(const std::string& filename) const
{
	const std::filesystem::path filepath = MinecraftServer::GetInstance().GetServerPath() / "world" / "structures" / (filename + ".nbt");
	if (!std::filesystem::exists(filepath))
	{
		return GetStructureSize("_default");
	}
	std::ifstream file(filepath.string(), std::ios::in | std::ios::binary);
	file.unsetf(std::ios::skipws);

	ProtocolCraft::NBT::Value nbt;
	file >> nbt;
	file.close();

	// TODO: deal with recursive structures (structures containing structure blocks) ?
	return nbt["size"].as_list_of<int>();
}

void TestManager::CreateTPSign(const Botcraft::Position& src, const Botcraft::Vector3<double>& dst, const std::vector<std::string>& texts, const std::string& facing, const std::string& color) const
{
	std::map<std::string, std::string> lines;
	for (size_t i = 0; i < std::min(texts.size(), 4ULL); ++i)
	{
		std::stringstream line;
		line
			<< "{"
			<< "\"text\":\"" << texts[i] << "\"" << ",";
		if (i == 0)
		{
			line
				<< "\"underlined\"" << ":" << "false" << ","
				<< "\"color\"" << ":" << "\"" << "black" << "\"" << ","
				<< "\"clickEvent\"" << ":" << "{"
					<< "\"action\"" << ":" << "\"run_command\"" << ","
					// TODO: add "facing" args on /teleport for 1.13+
					<< "\"value\"" << ":" << "\"teleport @s " << dst.x << " " << dst.y << " " << dst.z << "\""
				<< "}";
		}
		else
		{
			line
				<< "\"underlined\"" << ":" << "true" << ","
				<< "\"color\"" << ":" << "\"" << color << "\"";
		}
		line << "}";
		lines.insert({ "Text" + std::to_string(i + 1), line.str() });
	}

	int block_variant = 0;
	if (facing == "north")
	{
		block_variant = 0;
	}
	else if (facing == "south")
	{
		block_variant = 8;
	}
	else if (facing == "east")
	{
		block_variant = 12;
	}
	else if (facing == "west")
	{
		block_variant = 4;
	}

	SetBlock("standing_sign", src, block_variant, lines);
}

void TestManager::LoadStructure(const std::string& filename, const Botcraft::Position& pos, const Botcraft::Position& load_offset) const
{
	const std::string& loaded = std::filesystem::exists(MinecraftServer::GetInstance().GetServerPath() / "world" / "structures" / (filename + ".nbt")) ?
		filename :
		"_default";
	SetBlock("structure_block", pos, 0, {
		{"mode", "LOAD"},
		{"name", loaded},
		{"posX", std::to_string(load_offset.x)},
		{"posY", std::to_string(load_offset.y)},
		{"posZ", std::to_string(load_offset.z)},
		{"showboundingbox", "1"}
	});
	SetBlock("redstone_block", pos + Botcraft::Position(0, 1, 0));
}

void TestManager::MakeSureLoaded(const Botcraft::Position& pos) const
{
	// Don't teleport if we're close enough
	const double threshold_dist = Botcraft::CHUNK_WIDTH * (MinecraftServer::options.view_distance - 3);
	if (pos.SqrDist(chunk_loader_position) < threshold_dist * threshold_dist)
	{
		return;
	}
	Teleport(chunk_loader_name, pos);
	chunk_loader_position = pos;
}

void TestManager::testRunStarting(Catch::TestRunInfo const& test_run_info)
{
	// Make sure the server is running and ready before the first test run
	MinecraftServer::GetInstance().Initialize();
	// Retrieve header size
	header_size = GetStructureSize("_header_running");
	chunk_loader = GetBot(chunk_loader_name, chunk_loader_id, chunk_loader_position);
	SetGameMode(chunk_loader_name, Botcraft::GameType::Spectator);
}

void TestManager::testCaseStarting(Catch::TestCaseInfo const& test_info)
{
	// Retrieve test structure size
	current_test_size = GetStructureSize(test_info.name);
	LoadStructure("_header_running", Botcraft::Position(current_offset.x, 0, spacing_z - header_size.z));
	current_offset.z = 2 * spacing_z;
}

void TestManager::testCasePartialStarting(Catch::TestCaseInfo const& test_info, uint64_t part_number)
{
	// Load header
	current_header_position = current_offset - Botcraft::Position(0, 2, 0);
	LoadStructure("_header_running", current_header_position);
	current_offset.z += header_size.z;
	// Load test structure
	LoadStructure(test_info.name, current_offset + Botcraft::Position(0, -1, 0), Botcraft::Position(0, 1, 0));
	current_test_case_failures.clear();
	section_stack = { std::filesystem::path(test_info.lineInfo.file).stem().string() };
}

void TestManager::sectionStarting(Catch::SectionInfo const& section_info)
{
	section_stack.push_back(section_info.name);
}

void TestManager::assertionEnded(Catch::AssertionStats const& assertion_stats)
{
	if (!assertion_stats.assertionResult.succeeded())
	{
		const std::filesystem::path file_path = std::filesystem::relative(assertion_stats.assertionResult.getSourceInfo().file, BOTCRAFT_ROOT_SOURCE_DIR);
		current_test_case_failures.push_back(
			ReplaceCharacters(file_path.string(), {{'\\', "/"}}) + ":" +
			std::to_string(assertion_stats.assertionResult.getSourceInfo().line) + "\n\n" +
			assertion_stats.assertionResult.getExpressionInMacro()
		);
	}
}

void TestManager::testCasePartialEnded(Catch::TestCaseStats const& test_case_stats, uint64_t part_number)
{
	const bool passed = test_case_stats.totals.assertions.allPassed();
	// Replace header with proper test result
	LoadStructure(passed ? "_header_success" : "_header_fail", current_header_position);
	// Create TP sign for the partial that just ended
	CreateTPSign(
		Botcraft::Position(-2 * (current_test_index + 1), 2, -2 * (part_number + 2)),
		Botcraft::Vector3(current_offset.x, 2, current_offset.z - 1),
		section_stack, "north", passed ? "dark_green" : "red"
	);
	// Create back to spawn sign for the section that just ended
	CreateTPSign(
		Botcraft::Position(current_offset.x, 2, current_offset.z - 1),
		Botcraft::Vector3(-2 * (current_test_index + 1), 2, -2 * (static_cast<int>(part_number) + 2)),
		section_stack, "south", passed ? "dark_green" : "red"
	);
	if (!passed)
	{
		CreateBook(
			Botcraft::Position(current_offset.x + 1, 2, current_offset.z - header_size.z),
			current_test_case_failures,
			"north",
			test_case_stats.testInfo->name + "#" + std::to_string(part_number),
			"Botcraft Test Framework",
			section_stack
		);
	}
	else
	{
		const std::filesystem::path file_path = std::filesystem::relative(test_case_stats.testInfo->lineInfo.file, BOTCRAFT_ROOT_SOURCE_DIR);
		CreateBook(
			Botcraft::Position(current_offset.x + 1, 2, current_offset.z - header_size.z),
			{ ReplaceCharacters(file_path.string(), {{'\\', "/"}}) + ":" +
			std::to_string(test_case_stats.testInfo->lineInfo.line) + "\n\n" +
			"All passed! :)" },
			"north",
			test_case_stats.testInfo->name + "#" + std::to_string(part_number),
			"Botcraft Test Framework",
			section_stack
		);
	}
	current_offset.z += current_test_size.z + spacing_z;
}

void TestManager::testCaseEnded(Catch::TestCaseStats const& test_case_stats)
{
	const bool passed = test_case_stats.totals.assertions.allPassed();
	LoadStructure(passed ? "_header_success" : "_header_fail", Botcraft::Position(current_offset.x, 0, spacing_z - header_size.z));
	// Create Sign to TP to current test
	CreateTPSign(
		Botcraft::Position(-2 * (current_test_index + 1), 2, -2),
		Botcraft::Vector3(current_offset.x, 2, spacing_z - 1),
		{ std::filesystem::path(test_case_stats.testInfo->lineInfo.file).stem().string(), test_case_stats.testInfo->name },
		"north", passed ? "dark_green" : "red"
	);
	// Create sign to TP to TP back to spawn
	CreateTPSign(
		Botcraft::Position(current_offset.x, 2, spacing_z - 1),
		Botcraft::Vector3(-2 * (current_test_index + 1), 2, -2),
		{ std::filesystem::path(test_case_stats.testInfo->lineInfo.file).stem().string(), test_case_stats.testInfo->name },
		"south", passed ? "dark_green" : "red"
	);
	current_test_index += 1;
	current_offset.x += std::max(current_test_size.x, header_size.x) + spacing_x;
}

void TestManager::testRunEnded(Catch::TestRunStats const& test_run_info)
{
	if (chunk_loader)
	{
		chunk_loader->Disconnect();
	}
}




void TestManagerListener::testRunStarting(Catch::TestRunInfo const& test_run_info)
{
	TestManager::GetInstance().testRunStarting(test_run_info);
}

void TestManagerListener::testCaseStarting(Catch::TestCaseInfo const& test_info)
{
	TestManager::GetInstance().testCaseStarting(test_info);
}

void TestManagerListener::testCasePartialStarting(Catch::TestCaseInfo const& test_info, uint64_t part_number)
{
	TestManager::GetInstance().testCasePartialStarting(test_info, part_number);
}

void TestManagerListener::sectionStarting(Catch::SectionInfo const& section_info)
{
	TestManager::GetInstance().sectionStarting(section_info);
}

void TestManagerListener::assertionEnded(Catch::AssertionStats const& assertion_stats)
{
	TestManager::GetInstance().assertionEnded(assertion_stats);
}

void TestManagerListener::testCasePartialEnded(Catch::TestCaseStats const& test_case_stats, uint64_t part_number)
{
	TestManager::GetInstance().testCasePartialEnded(test_case_stats, part_number);
}

void TestManagerListener::testCaseEnded(Catch::TestCaseStats const& test_case_stats)
{
	TestManager::GetInstance().testCaseEnded(test_case_stats);
}

void TestManagerListener::testRunEnded(Catch::TestRunStats const& test_run_info)
{
	TestManager::GetInstance().testRunEnded(test_run_info);
}
CATCH_REGISTER_LISTENER(TestManagerListener)

std::string ReplaceCharacters(const std::string& in, const std::vector<std::pair<char, std::string>>& replacements)
{
	std::string output;
	output.reserve(in.size());

	for (size_t i = 0; i < in.size(); ++i)
	{
		bool found = false;
		for (size_t j = 0; j < replacements.size(); ++j)
		{
			if (replacements[j].first == in[i])
			{
				output += replacements[j].second;
				found = true;
				break;
			}
		}
		if (!found)
		{
			output += in[i];
		}
	}

	return output;
}
