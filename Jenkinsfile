// Powered by Infostretch 

timestamps {

node () {

	stage ('test-project-c++-build - Checkout') {
 	 checkout([$class: 'GitSCM', branches: [[name: '*/master']], doGenerateSubmoduleConfigurations: false, extensions: [], submoduleCfg: [], userRemoteConfigs: [[credentialsId: 'deanone_github_credentials', url: 'https://github.com/deanone/Gaussian-elimination.git']]]) 
	}
	stage ('test-project-c++-build - Build') {
 			// Shell build step
sh """ 
./compile.sh 
 """ 
	}
	stage ('test-project-c++-deploy - Build') {
 			// Shell build step
sh """ 
./gaussianElimination.out 4 4 0 1 1 
 """ 
	}
}
}