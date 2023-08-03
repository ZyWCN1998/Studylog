# 20230803 Missing Semester 05

## SSH

Install a Linux virtual machine (or use an already existing one) for this exercise. If you are not familiar with virtual machines check out [this](https://hibbard.eu/install-ubuntu-virtual-box/) tutorial for installing one.

直接使用我的云服务器做Remote Server即可

```bash
❯ ssh ubuntu@49.233.53.117
```

1.  Go to `~/.ssh/` and check if you have a pair of SSH keys there. If not, generate them with `ssh-keygen -o -a 100 -t ed25519`. It is recommended that you use a password and use `ssh-agent` , more info [here](https://www.ssh.com/ssh/agent).

```bash
❯ ssh-keygen -o -a 100 -t ed25519
❯ cat ~/.ssh/id_ed25519.pub | ssh ubuntu@49.233.53.117 'cat >> ~/.ssh/authorized_keys'
```

2. Edit `.ssh/config` to have an entry as follows

 `Host vm
     User username_goes_here
     HostName ip_goes_here
     IdentityFile ~/.ssh/id_ed25519
     LocalForward 9999 localhost:8888`

按照我自己云服务器地址等设置

```bash
.ssh
├── config
├── id_ed25519
├── id_ed25519.pub
├── known_hosts
└── known_hosts.old
```

```bash
Host vm
     User ubuntu
     HostName 49.233.53.117
     IdentityFile ~/.ssh/id_ed25519
     LocalForward 9999 localhost:8888
```

3. Use `ssh-copy-id vm` to copy your ssh key to the server.

显示以下内容的原因是已经在服务器上配置了id

```bash
❯ ssh-copy-id vm
/usr/bin/ssh-copy-id: INFO: attempting to log in with the new key(s), to filter out any that are already installed

/usr/bin/ssh-copy-id: ERROR: ssh: Could not resolve hostname ip_goes_here: Temporary failure in name resolution
```

4. Start a webserver in your VM by executing `python -m http.server 8888`. Access the VM webserver by navigating to `http://localhost:9999` in your machine.

```bash
python3 -m http.server 8888
Serving HTTP on 0.0.0.0 port 8888 (http://0.0.0.0:8888/) ...

127.0.0.1 - - [03/Aug/2023 20:43:44] "GET / HTTP/1.1" 200 -
127.0.0.1 - - [03/Aug/2023 20:43:44] code 404, message File not found
127.0.0.1 - - [03/Aug/2023 20:43:44] "GET /favicon.ico HTTP/1.1" 404 -
127.0.0.1 - - [03/Aug/2023 20:43:45] "GET / HTTP/1.1" 200 -
127.0.0.1 - - [03/Aug/2023 20:43:46] "GET / HTTP/1.1" 200 -
127.0.0.1 - - [03/Aug/2023 20:43:46] "GET / HTTP/1.1" 200 -
127.0.0.1 - - [03/Aug/2023 20:43:46] "GET / HTTP/1.1" 200 -
127.0.0.1 - - [03/Aug/2023 20:43:47] "GET / HTTP/1.1" 200 -
127.0.0.1 - - [03/Aug/2023 20:44:14] "GET /id_ed25519 HTTP/1.1" 200 -
```

5. Edit your SSH server config by doing `sudo vim /etc/ssh/sshd_config` and disable password authentication by editing the value of `PasswordAuthentication`. Disable root login by editing the value of `PermitRootLogin`. Restart the `ssh` service with `sudo service sshd restart`. Try sshing in again.

修改对应位置值即可

```bash
# This is the ssh client system-wide configuration file.  See
# ssh_config(5) for more information.  This file provides defaults for
# users, and the values can be changed in per-user configuration files
# or on the command line.

# Configuration data is parsed as follows:
#  1. command line options
#  2. user-specific file
#  3. system-wide file
# Any configuration value is only changed the first time it is set.
# Thus, host-specific definitions should be at the beginning of the
# configuration file, and defaults at the end.

# Site-wide defaults for some commonly used options.  For a comprehensive
# list of available options, their meanings and defaults, please see the
# ssh_config(5) man page.

Include /etc/ssh/ssh_config.d/*.conf

Host *
#   ForwardAgent no
#   ForwardX11 no
#   ForwardX11Trusted yes
#   PasswordAuthentication yes
#   HostbasedAuthentication no
#   GSSAPIAuthentication no
#   GSSAPIDelegateCredentials no
#   GSSAPIKeyExchange no
#   GSSAPITrustDNS no
#   BatchMode no
#   CheckHostIP yes
#   AddressFamily any
#   ConnectTimeout 0
#   StrictHostKeyChecking ask
#   IdentityFile ~/.ssh/id_rsa
#   IdentityFile ~/.ssh/id_dsa
#   IdentityFile ~/.ssh/id_ecdsa
#   IdentityFile ~/.ssh/id_ed25519
#   Port 22
#   Ciphers aes128-ctr,aes192-ctr,aes256-ctr,aes128-cbc,3des-cbc
#   MACs hmac-md5,hmac-sha1,umac-64@openssh.com
#   EscapeChar ~
#   Tunnel no
#   TunnelDevice any:any
#   PermitLocalCommand no
#   VisualHostKey no
#   ProxyCommand ssh -q -W %h:%p gateway.example.com
#   RekeyLimit 1G 1h
#   UserKnownHostsFile ~/.ssh/known_hosts.d/%k
    SendEnv LANG LC_*
    HashKnownHosts yes
    GSSAPIAuthentication yes
```

6. (Challenge) Install `[mosh](https://mosh.org/)` in the VM and establish a connection. Then disconnect the network adapter of the server/VM. Can mosh properly recover from it?

```bash
sudo apt install mosh
mosh ubuntu@49.233.53.117
```

断开网络连接之后谁都不可能连上，恢复网络连接之后mosh只是延迟更低一些

7. (Challenge) Look into what the `-N` and `-f` flags do in `ssh` and figure out a command to achieve background port forwarding.

```bash
-N      Do not execute a remote command.  This is useful for just forwarding ports.

-f      Requests ssh to go to background just before command execution.  This is useful if ssh is going to ask for passwords or passphrases, but the user wants it in the background.  This implies -n.  The recommended way to start X11 programs at a remote site is with something like ssh -f host xterm.

If the ExitOnForwardFailure configuration option is set to ``yes'', then a client started with -f will wait for all remote port forwards to be successfully established before placing itself in the background.
```