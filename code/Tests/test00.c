/**
    Correct behavior:
    Debug stream should show the word "true".
*/
task main()
{
    bool fTest = true;
    writeDebugStreamLine(fTest? "true": "false");
}
