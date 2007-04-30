#!/usr/local/bin/perl
#
# BENCHMARK.PL
#  caleb brown, 2003
#
# This perl script is used for benchmarking the gloffview program.

$seconds_per_run = 10;
$small_window = 300;
$big_window = 800;
$small_off = "../../off/tref.off";
$big_off = "../../off/harley.off";
$runsize = 6;

## STATISTICS::
## 24 combinations of parameters
## 6 + 1 runs per parameter
## 10 seconds per run
## 240 * 7 = 1680
## 1680 seconds per video card per computer
## 1680s = 28m

$fixed_params = "-r x -a 1 -c $seconds_per_run";
@oparams = ("-o n","-o d","-o v");
@bparams = ("-b","");
@wparams = ("-w $small_window", "-w $big_window");
@fparams = ($small_off,$big_off);

$total_runs = 1;

for($oindex = 0; $oindex <= $#oparams; $oindex++) {
  for($bindex = 0; $bindex <= $#bparams; $bindex++) {
    for($windex = 0; $windex <= $#wparams; $windex++) {
	  for($findex = 0; $findex <= $#fparams; $findex++) {
	    # build up the command to execute
	    $command = "gloffview $fixed_params $oparams[$oindex] $bparams[$bindex] $wparams[$windex] $fparams[$findex]";

		# print out a message
        print "$total_runs: $command\n";

		# run the command once and throw away the result
		$dummy = `$command`;

		# now run the command multiple times, storing the biggest, smallest and tally
		$biggest = 0;
		$smallest = 0;
		$tally = 0;
		print "$total_runs: ";
		for($run = 0; $run < $runsize; $run++){
		  $result = `$command`;
		  chomp($result);
		  $result =~ s/^FPS = //;

		  # drop the smallest and the biggest results
		  if(($result < $smallest) || ($smallest == 0)) {
		    $tally += $smallest;
		    $smallest = $result;
		  } elsif($result > $biggest){
		    $tally += $biggest;
		    $biggest = $result;
		  } else {
		    $tally += $result;
		  }

		  print "$result ";
		}
		$avgfps = $tally / ($runsize-2);
		print "\n$total_runs: Smallest = $smallest  Biggest = $biggest  AVG FPS = $avgfps\n\n";
		$total_runs++;
      }
	}
  }
}
