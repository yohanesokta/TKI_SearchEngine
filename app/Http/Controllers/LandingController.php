<?php
namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Symfony\Component\Process\Process;
use Symfony\Component\Process\Exception\ProcessFailedException;

class LandingController extends Controller
{
    public function search(Request $request)
    {
        $query = $request->input('query', ''); 
        $limit = $request->input('limit', 10); 

        if (empty($query)) {
            return response()->json([
                'error' => 'Query parameter is required'
            ], 400);
        }
        
        $pythonScript = base_path('scrapper\\query.py'); 
        $pickleFile = base_path('scrapper\\linux_dataset.bin'); 

        
        $escapedQuery = escapeshellarg($query);
        $escapedLimit = intval($limit);

        
        $cmd = "python3 {$pythonScript} {$pickleFile} {$escapedLimit} {$escapedQuery}";
        exec($cmd, $output, $return_var);

        if ($return_var !== 0) {
            return response()->json([
                'dir' => $pythonScript,
                'error' => 'Python script failed',
                'output' => $output
            ], 500);
        }

        $results = [];
        foreach ($output as $line) {
            $decoded = json_decode($line, true);
            if ($decoded) {
                $results[] = $decoded;
            }
        }

        return response()->json([
            'query' => $query,
            'results' => $results
        ]);
    }
}