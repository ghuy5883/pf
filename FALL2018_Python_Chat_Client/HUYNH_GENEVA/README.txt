TO DEMO ON COMMAND LINE:
python h1_pt3_server.py -p 5000 -l logfile.txt -o 6000
(for the server1)
python h1_pt3_server.py -p 5001 -l logfile2.txt -o 6001 -t 6000
(for the server2)
python hw1_pt3_client.py -p 5000 -l logfileclient.txt -n NAME1
(for the client)
python hw1_pt3_client.py -p 5001 -l logifileclient2.txt -n NAME2
(for the 2nd client)
