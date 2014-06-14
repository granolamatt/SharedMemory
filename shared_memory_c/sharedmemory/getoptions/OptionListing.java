/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sharedmemory.getoptions;

import java.util.LinkedList;

/**
 *
 * @author root
 */
public class OptionListing {

    private static final int name = 0;
    private static final int description = 1;
    private static final int shortName = 2;
    private static final int args = 3;
    private static String progName = "java";
    private static final String[][] opts
            = {
                {"size      ", "Amount of shared memory", "s", "1"},
                {"name      ", "Shared name to attach to", "n", "1"},
                {"read      ", "Read the memory", "r", null},
                {"close     ", "Close the memory and symaphore", "c", null}
            };

    private static int option_index = 0;

    public static class RetClass {

        public int index;
        public String[] args;

        public RetClass() {

        }

        public RetClass(int index) {
            this.index = index;
        }

        public RetClass(int index, String[] args) {
            this.index = index;
            this.args = args;
        }
    }

    /* int getopts_usage()
     *
     *  Returns: 1 - Successful
     */
    public static RetClass getopts_usage() {
        int count;

        System.out.println("Usage: " + progName + " [options]\n");
        System.out.println("  --help,\t-h\t\t\tDisplays this information");
        for (count = 0; count < opts.length; count++) {
            String cmd = "";
            if (opts[count][name] != null && opts[count][shortName] != null) {
                if (opts[count][args] != null) {
                    cmd = "--" + opts[count][name] + ",\t\t-" + opts[count][shortName] + " <args>\t\t";
                } else {
                    cmd = "--" + opts[count][name] + ",\t\t-" + opts[count][shortName] + "\t\t\t";
                }
            } else if (opts[count][name] != null) {
                if (opts[count][args] != null) {
                    cmd = "--" + opts[count][name] + " <args>\t\t\t";
                } else {
                    cmd = "--" + opts[count][name] + "\t\t\t\t";
                }
            } else if (opts[count][shortName] != null) {
                if (opts[count][args] != null) {
                    cmd = "\t\t-" + opts[count][shortName] + " <args>\t\t";
                } else {
                    cmd = "\t\t-" + opts[count][shortName] + "\t\t\t";
                }
            }
            System.out.println(cmd + opts[count][description]);
        }
        return null;
    }

    public static RetClass[] parseAllOptions(String[] args) {

        LinkedList<OptionListing.RetClass> argList = new LinkedList<>();
        OptionListing.RetClass argv = OptionListing.getopts(args);
        while (argv != null) {
            argList.add(argv);
            argv = OptionListing.getopts(args);
        }

        RetClass[] ret = new RetClass[argList.size()];

        for (int cnt = 0; cnt < ret.length; cnt++) {
            ret[cnt] = argList.removeFirst();
        }
        return ret;
    }

    /* int getopts()
     *
     * Returns: -1 - Couldn't allocate memory.  Please handle me. 
     *          0  - No arguements to parse
     *          #  - The number in the struct for the matched arg.
     *
     */
    public static RetClass getopts(String[] argv) {

        if (argv.length > 0) {
            if (option_index == 0 && !argv[0].startsWith("-")) {
                progName = argv[0];
                option_index++;
            }
        }
        for (String arg : argv) {
            if (arg.equals("-h") || arg.equals("--help")) {
                getopts_usage();
                System.exit(0);
                return null;
            }
        }
        RetClass retargs = new RetClass();
        retargs.args = new String[1];
        retargs.args[0] = "";
        for (int cnt = option_index; cnt < argv.length; cnt++) {
//            System.out.println("Checking " + argv[cnt] + " " + argv[cnt].substring(1));
            if (argv[cnt].startsWith("--")) {
                for (int idx = 0; idx < opts.length; idx++) {
                    String[] obj = opts[idx];
                    if (obj[name].trim().equals(argv[cnt].substring(2))) {
                        if (argv.length > cnt && obj[args] != null) {
                            retargs.args[0] = argv[cnt + 1];
                            option_index++;
                        }
                        option_index++;
                        retargs.index = idx;
                        return retargs;
                    }
                }
            } else if (argv[cnt].startsWith("-")) {
                for (int idx = 0; idx < opts.length; idx++) {
                    String[] obj = opts[idx];
                    if (obj[shortName].trim().equals(argv[cnt].substring(1))) {
                        if (argv.length > cnt && obj[args] != null) {
                            retargs.args[0] = argv[cnt + 1];
                            option_index++;
                        }
                        option_index++;
                        retargs.index = idx;
                        return retargs;

                    }
                }
                getopts_usage();
                System.exit(0);
            }
        }
        option_index++;
        return null;
    }

}
