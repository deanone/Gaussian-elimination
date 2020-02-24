pipeline {
   agent any

   stages {
      stage('Build') {
          steps {
            sh """ ./compile.sh  """ 
          }
      }
      stage('Deploy') {
          steps {
            sh """ ./gaussianElimination.out 4 4 0 1 1 """ 
          }
      }
   }
}
