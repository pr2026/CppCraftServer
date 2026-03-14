#include "terminal.h"
#include "error.h"

#include <cstdio>
#include <string>
#include <sys/wait.h>

Terminal_result terminal(const std::string& command, const std::string& input) {
    char buffer[128];

    Terminal_result result;
    result.exit_code = 0;

    std::string full_command;
    if (input.empty()) {
    full_command = "printf '' | " + command;
    } else {
    full_command = "printf '%s' '" + input + "' | " + command;
    }   
 
  FILE *pipe = popen(full_command.c_str(), "r");

  if (!pipe) {
    result.error_msg = "ERROR: Failed to run command";
    result.exit_code = -1;
    return result;
  }

  while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
    result.output += buffer;
  }

  int status = pclose(pipe);
  if (WIFEXITED(status)) {
    result.exit_code = WEXITSTATUS(status);
    if (result.exit_code != 0) {
    result.error_msg = error(result.exit_code);
  }
} else if (WIFSIGNALED(status)) {
    int signal = WTERMSIG(status); 
    result.exit_code = 128 + signal;
    result.error_msg = error(result.exit_code); 
  }
  return result;
}