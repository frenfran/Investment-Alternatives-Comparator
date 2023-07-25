# Investment Alternatives Comparator (InAC)

A terminal-based, command-line interface application program for ranking multiple investment alternatives based on four different methods of analysis. Includes the ability to dynamically modify, remove and create new alternatives prior to analysis. Subsequent ranking of investment alternatives is done via either the:
- net present value
- net annual value
- net future value
- incremental internal rate of return algorithm

It is recommended to use the incremental internal rate of return method when only comparing two alternatives, or, when only the best investment alternative is to be selected. Additionally, although the first three methods of analysis will display either the present, annual or future values respectively for each alternative, the incremental internal rate of return method will display the *individual* IRR values for each alternative, with a margin of error no greater than $1. In the event that the program is unable to compute a project's IRR value, an asterisk (*) will be displayed instead. When in doubt, always use the net present value method of analysis.
