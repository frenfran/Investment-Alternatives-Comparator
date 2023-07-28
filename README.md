# Investment Alternatives Comparator (InAC)

A terminal-based, command-line interface application program for ranking multiple investment alternatives based on four different methods of analysis. Includes the ability to dynamically modify, remove and create new alternatives prior to analysis. Subsequent ranking of investment alternatives is done via either the:
- net present value
- net annual value
- net future value
- incremental internal rate of return algorithm

It is recommended to use the incremental internal rate of return method when only comparing two alternatives, or, when only the best investment alternative is to be selected. Additionally, although the first three methods of analysis will display either the present, annual or future values respectively for each alternative, the incremental internal rate of return method will display the *individual* IRR values for each alternative, with a margin of error no greater than $1. In the event that the program is unable to compute a project's IRR value, an asterisk (*) will instead be shown in its place. When in doubt, always use the net present value method of analysis.

The following is a series of examples, each consisting of both the problem statement as well as the execution of the program. All problem statements are courtesy of Oxford University Press and can be found here: [Problem Statements](https://global.oup.com/us/companion.websites/9780199772766/student/pdf/Chapter9E2010.pdf) (note that certain sections have been modified to increase clarity).

### Example 1
>The Tennessee Department of Highways is trying to decide whether it should “hot-patch” a short stretch of an existing highway or resurface it. If the hot-patch method is chosen, approximately 500 cubic meters of material would be required at a cost of $800/cubic meter (in place). If hotpatched, the shoulders will have to be improved at the same time at a cost of $24,000. The shoulders must be maintained at a cost of $3,000 every two years. The annual cost of routine
maintenance on the patched road is estimated to be $6,000.
Alternatively, the state can resurface the road at a cost of $500,000. This surface will last 10 years if maintained properly at a cost of $2,000 per year beginning in the second year. The shoulders would require reworking at the end of the fifth year at a cost of $15,000. Regardless of the method selected the road will be completely rebuilt in 10 years. At an interest rate of 9%, which alternative should be chosen?

Program execution for Example 1: [Example1.txt](https://github.com/frenfran/Investment-Alternatives-Comparator/files/12196743/Example1.txt)

### Example 2
>Warren Buffett is considering investing in one of the below investment projects. Each alternative has a five-year useful life. If Warren requires at least an 8% return on his investment, which single alternative should he select?

| Investment | A | B | C | D | E |
| :--- | :---: | :---: | :---: | :---: | :---: |
| First Cost | $100.00 | $130.00 | $200.00 | $330.00 | $0.00 | 
| Annual Income | $150.00 | $130.78 | $185.00 | $184.55 | $0.00 |
| Annual Cost | $123.62 | $92.00 | $137.52 | $93.00 | $0.00 | 

Program execution for Example 2: [Example2.txt](https://github.com/frenfran/Investment-Alternatives-Comparator/files/12197151/Example2.txt)

### Example 3
>Morton & Moore LLC (M&M) is trying to decide between two machines which are necessary in their manufacturing facility. Data concerning the two machines are presented below. If M&M has a minimum attractive rate of return (MARR) of 15%, which machine should be chosen?

| Machine | A | B |
| :--- | :---: | :---: |
| First Cost | $45,000 | $24,000 |
| Annual Operating Costs | $31,000 | $35,000 |
| Overhaul in Years 2 and 4 | - | $6,000 |
| Overhaul in Year 5 | $12,000 | - |
| Salvage Value | 10,000 | 8,000 |
| Useful Life | 8 years | 6 years |

Program execution for Example 3: [Example3.txt](https://github.com/frenfran/Investment-Alternatives-Comparator/files/12197154/Example3.txt)

Note that the depreciation rate was determined by solving for x in the following equation and taking the answer between zero and one: $FC * (1-x)^L = SV$ where FC is first cost, L is useful life and SV is salvage value.
