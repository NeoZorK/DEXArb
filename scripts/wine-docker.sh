#!/bin/bash

# Wine Docker Container Management Script

set -e

CONTAINER_NAME="wine-ubuntu"
COMPOSE_FILE="docker-compose.wine.yml"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if Docker is running
check_docker() {
    if ! docker info >/dev/null 2>&1; then
        print_error "Docker is not running. Please start Docker first."
        exit 1
    fi
}

# Function to check if docker-compose is available
check_compose() {
    if ! command -v docker-compose >/dev/null 2>&1; then
        print_error "docker-compose is not installed. Please install it first."
        exit 1
    fi
}

# Function to build and start the container
start() {
    print_status "Building and starting Wine container..."
    docker-compose -f "$COMPOSE_FILE" up -d --build
    print_status "Container started successfully!"
    print_status "To access the container, run: $0 shell"
}

# Function to stop the container
stop() {
    print_status "Stopping Wine container..."
    docker-compose -f "$COMPOSE_FILE" down
    print_status "Container stopped successfully!"
}

# Function to restart the container
restart() {
    print_status "Restarting Wine container..."
    docker-compose -f "$COMPOSE_FILE" restart
    print_status "Container restarted successfully!"
}

# Function to access the container shell
shell() {
    print_status "Accessing Wine container shell..."
    docker exec -it "$CONTAINER_NAME" /bin/bash
}

# Function to run a Windows executable
run_exe() {
    if [ -z "$1" ]; then
        print_error "Please provide the path to the Windows executable"
        print_status "Usage: $0 run <path_to_exe>"
        exit 1
    fi
    
    print_status "Running Windows executable: $1"
    docker exec -it "$CONTAINER_NAME" wine "$1"
}

# Function to show container status
status() {
    print_status "Container status:"
    docker-compose -f "$COMPOSE_FILE" ps
}

# Function to show container logs
logs() {
    print_status "Container logs:"
    docker-compose -f "$COMPOSE_FILE" logs -f
}

# Function to clean up everything
cleanup() {
    print_warning "This will remove the container and all data. Are you sure? (y/N)"
    read -r response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
        print_status "Cleaning up..."
        docker-compose -f "$COMPOSE_FILE" down -v
        docker system prune -f
        print_status "Cleanup completed!"
    else
        print_status "Cleanup cancelled."
    fi
}

# Function to show help
show_help() {
    echo "Wine Docker Container Management Script"
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo "Commands:"
    echo "  start     - Build and start the Wine container"
    echo "  stop      - Stop the Wine container"
    echo "  restart   - Restart the Wine container"
    echo "  shell     - Access the container shell"
    echo "  run <exe> - Run a Windows executable"
    echo "  status    - Show container status"
    echo "  logs      - Show container logs"
    echo "  cleanup   - Remove container and all data"
    echo "  help      - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 start"
    echo "  $0 shell"
    echo "  $0 run /app/program.exe"
}

# Main script logic
main() {
    check_docker
    check_compose
    
    case "${1:-help}" in
        start)
            start
            ;;
        stop)
            stop
            ;;
        restart)
            restart
            ;;
        shell)
            shell
            ;;
        run)
            run_exe "$2"
            ;;
        status)
            status
            ;;
        logs)
            logs
            ;;
        cleanup)
            cleanup
            ;;
        help|--help|-h)
            show_help
            ;;
        *)
            print_error "Unknown command: $1"
            show_help
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@"
