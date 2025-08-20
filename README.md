Projeto Cliente-Servidor – Produtor e Supervisor

Este projeto foi desenvolvido para a disciplina Programação Avançada e tem como objetivo implementar a comunicação entre clientes e um servidor utilizando o modelo cliente-servidor.

🎯 Descrição do Projeto

O sistema é composto por três entidades principais:

Servidor

Responsável por gerenciar a comunicação entre os clientes.

Mantém o endereço de conexão que permite o envio e recebimento de mensagens.

Cliente Produtor

Gera e envia dados ao servidor.

Os dados produzidos são repassados para o cliente supervisor através da mediação do servidor.

Cliente Supervisor

Recebe os dados enviados pelo cliente produtor via servidor.

Atua como monitor, garantindo que as informações transmitidas estejam corretas.

⚙️ Funcionamento

O Cliente Produtor se conecta ao Servidor utilizando o endereço definido.

Os dados produzidos são enviados para o servidor.

O Servidor encaminha esses dados ao Cliente Supervisor.

O Cliente Supervisor recebe e processa as informações.
