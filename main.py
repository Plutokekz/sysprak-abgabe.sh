import requests
import os
import subprocess
from subprocess import Popen, PIPE
import time
import json

#"Content-Type: application/json" -d '{"type":"Bashni","errorMsg":"","gameGeneric":{"name":"","timeout":3000},"gameSpecific":{},"players":[{"name":"","type":"COMPUTER","gameSpecific":{}},{"name":"","type":"HUMAN","gameSpecific":{}}]}'

def make_request():
  url = 'http://sysprak.priv.lab.nm.ifi.lmu.de/api/v1/matches'
  payload = {"type":"Bashni",
             "errorMsg":"PeterZwegat",
             "gameGeneric": {
               "name":"Training-01",
               "timeout":3000
             },
             "gameSpecific":{},
             "players":[
               {
                 "name":"White",
                 "type":"COMPUTER",
                 "gameSpecific":{}
               },
               {
                 "name":"Black",
                 "type":"COMPUTER",
                 "gameSpecific":{}
               }
             ]}
  headers = {'content-type': 'application/json'}

  r = requests.post(url, data=json.dumps(payload), headers=headers)

  return r


if __name__ == "__main__":
    r = make_request()
    game_id = r.json()['id']
    print(f"http://sysprak.priv.lab.nm.ifi.lmu.de/bashni/#{game_id}")
    subprocess.run(["firefox",  f"http://sysprak.priv.lab.nm.ifi.lmu.de/bashni/#{game_id}"])
    path = os.getcwd()
    # p = Popen(['watch', 'ls'])
    player_1 = Popen([f"./sysprak-client -g {game_id}"], shell=True)
    player_2 = Popen([f"./sysprak-client -g {game_id}"], shell=True)

    """running_procs = [player_1, player_2]

    while running_procs:
      for proc in running_procs:
        retcode = proc.poll()
        if retcode is not None: # Process finished.
            running_procs.remove(proc)
            break
        else: # No process is done, wait a bit and check again.
            time.sleep(.1)
            continue

    # Here, `proc` has finished with return code `retcode`
    if retcode != 0:
      pass
    print(proc.stdout)"""

