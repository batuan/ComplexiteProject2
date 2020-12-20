cd "./test/datagraph/"
yourfilenames=`ls *.txt`
cd "../.."
for eachfile in $yourfilenames
do
   ./mini2 $eachfile 3
done